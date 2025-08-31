from time import time

from math import sqrt,log

from random import choice,shuffle,randint

import numpy as np

from helper import *

from copy import deepcopy


class MCTS:

    class Node:

        def __init__(self,state:np.array,player_number:int,parent:'MCTS.Node'=None,parent_action:Tuple[int,int]=(-1,-1)) -> None:

            self.state=deepcopy(state)

            self.player_number=player_number

            self.parent=parent

            self.parent_action=parent_action

            self.children={}

            self.n=0

            self.t=0

            self.n_rave=0

            self.t_rave=0


        def is_leaf(self):

            return len(self.children)==0


        def is_unexplored(self):

            return self.n==0


    class UnionFind:

        def __init__(self,n:int) -> None:

            self.rank=[1 for i in range(n)]

            self.parent=[i for i in range(n)]


        def find(self,i:int) -> int:

            while i!=self.parent[i]:

                self.parent[i]=self.parent[self.parent[i]]

                i=self.parent[i]

            return i


        def union(self,i:int,j:int) -> bool:

        # return if two nodes are in different SCC, and merges them

            i,j=self.find(i),self.find(j)

            if i!=j:

                if self.rank[i]>=self.rank[j]:

                    self.parent[j]=i

                    self.rank[i]+=self.rank[j]

                    return self.rank[i]

                else:

                    self.parent[i]=j

                    self.rank[j]+=self.rank[i]

                    return self.rank[j]

            else: return -1


    def __init__(self,state:np.array,player_number:int,p1:list[list[list[Tuple[int, int]]]],try_s:int,confidence:float) -> None:

        self.state=state

        self.dim=len(state)

        self.player_number=player_number

        self.opp=3-self.player_number

        self.root=self.Node(state,player_number)

        for move in get_valid_actions(state):

            child=self.Node(state,self.opp,self.root,move)

            child.state[move[0]][move[1]]=self.player_number

            self.root.children[move]=child


        self.gorandom=try_s<0

        self.confidence=confidence

        self.p1=p1

        self.a=sqrt(3)

        self.k=1300.0

        self.boundary=get_all_corners(self.dim)

        for i in get_all_edges(self.dim):

            self.boundary+=i


    def choose(self,state:np.array,it:int)->Tuple[int,int]:


        test,act=self.one_move_lose()

        if test:

            # print(f'AI moved {act} during one move check')

            return act

        elif not self.gorandom:

            s_move=self.hardcode()

            if not self.gorandom:

                # print(f'AI moved {s_move} using hard-code')

                return s_move



        for self.iter in range(it): self.iteration()


        x=max(self.root.children.values(),key=self.pick).parent_action

        # print(f'AI moved {x} using AI')

        return x


    def hardcode(self)->Tuple[int,int]:

        if self.gorandom: return (-1,-1)

        pieces=set(get_valid_actions(self.state))

        while self.p1:

            line=self.p1[-1]

            if len(line)==0:

                self.p1.pop()

                continue

            ok=True

            for block in line:

                for pos in block[:]:

                    if pos not in pieces:

                        block.remove(pos)

                if not block:

                    self.p1.pop()

                    ok=False

                    shuffle(self.p1)

                    break

            if ok:

                shuffle(line)

                for x in line:

                    if len(x)==1 and self.state[x[0][0]][x[0][1]]==0:

                        line.remove(x)

                        return x[0]

                opn=line.pop()

                for x in opn:

                    if self.state[x[0]][x[1]]==0:

                        return x


        self.gorandom=True

        return (-1,-1)


    def one_move_lose(self):

        for p in get_valid_actions(self.state):

            a,b=p

            self.state[a][b]=self.player_number

            if check_win(self.state,p,self.player_number)[0]:

                self.state[a][b]=0

                return (True,p)


            self.state[a][b]=self.opp

            if check_win(self.state,p,self.opp)[0]:

                self.state[a][b]=0

                return (True,p)


            # for a1,b1 in get_valid_actions(self.state):

            #     self.state[a1][b1]=self.opp

            #     if check_win(self.state,(a1,b1),self.opp)[0]:

            #         self.state[a][b]=0

            #         self.state[a1][b1]=0

            #         return (True,(a1,b1))

            #     self.state[a1][b1]=0


            self.state[a][b]=0

        return (False,(-1,-1))


    def ucb(self,node:Node)->float:

        if node.is_unexplored(): return float('inf')

        else:

            alpha = self.k/(self.k+node.n)

            # alpha = 0

            UCT = float(node.t)/float(node.n)+self.a*sqrt(log(node.parent.n)/node.n)

            AMAF= float(node.t_rave) / float(node.n_rave) if node.n_rave!=0 else 0

            return (1 - alpha) * UCT + alpha * AMAF


    def pick(self,node:Node)->float:

        def edging()->int:

            board= (node.state==self.player_number)

            move=node.parent_action

            group=set(bfs_reachable(board,move))

            bonus=0


            for edge_number in range(6):

                for vertex in get_vertices_on_edge(edge_number,self.dim):

                    if vertex in group:

                        bonus+=1

                        break


            for corner_number in range(6):

                if get_vetex_at_corner(corner_number,self.dim) in group:

                    bonus+=1



            return bonus


        def connectedness()->int:

            n=len(node.state)

            dsu=self.UnionFind(n*n)

            x,y=node.parent_action

            node.state[x][y]=0

            for i in range(n):

                for j in range(n):

                    if self.state[i][j]==self.player_number:

                        for a,b in get_neighbours(self.dim,(i,j)):

                            if node.state[a][b]==self.player_number:

                                dsu.union(n*i+j,n*a+b)


            nebor={}

            for u,v in get_neighbours(self.dim,(x,y)):

                if node.state[u][v]==self.player_number:

                    nebor[dsu.parent[u*n+v]]=dsu.rank[dsu.find(u*n+v)]

            node.state[x][y]=self.player_number

            return sum(nebor.values()) if len(nebor)>1 else 0


        def locality()->int:

            a, b = node.parent_action

            bonus=0

            for x,y in get_neighbours(self.dim,(a,b)):

                if node.state[x][y]==self.player_number:

                    bonus+=2


            def add(s1,s2)->Tuple[int,int]:

                dx1,dy1=move_coordinates(s1,b-self.dim//2)

                a1,b1=a+dx1,b+dy1

                dx2,dy2=move_coordinates(s2,b1-self.dim//2)

                a2,b2=a1+dx2,b1+dy2

                return (a2,b2)


            second_neighbors=[add('up','up'),add('down','down'),add('top-right','top-right'),add('top-left','top-left'),add('bottom-left','bottom-left'),add('bottom-right','bottom-right')]


            virtual_moves=[add('top-left','up'),add('top-right','up'),add('top-right','bottom-right'),add('top-left','bottom-left'),add('down','bottom-left'),add('down','bottom-right')]


            for x,y in second_neighbors:

                if 0 <= x < self.state.shape[0] and 0 <= y < self.state.shape[1] and bonus>0:

                    if self.state[x][y]==self.player_number:

                        bonus+= 3


            for x,y in virtual_moves:

                if 0 <= x < self.state.shape[0] and 0 <= y < self.state.shape[1] and bonus>0:

                    if self.state[x][y]==self.player_number:

                        bonus+= 5


            return bonus


        ucb=self.ucb(node)

        local=locality()

        group=connectedness()

        connection=edging()

        if local+group+connection==0:ucb/=2

        return ucb*(1+(2.0/(1.0+self.confidence))*(0.5*connection+1.2*group)*(1+0.5*local))


    def traversal(self)->Node:

        current=self.root

        while not current.is_leaf():

            current=max(current.children.values(),key=self.ucb)

        return current


    def expansion(self,node:Node)->Node:

        for a,b in get_valid_actions(node.state):

            child=self.Node(node.state,3-node.player_number,node,(a,b))

            child.state[a][b]=node.player_number

            node.children[(a,b)]=child

        return choice(list(node.children.values())) if node.children else node


    def rollout(self,state:np.array,player:int) -> Tuple[int,list[Tuple[int,int]],list[Tuple[int,int]]]:

        # return the tuple outcome,black_rave,white_rave

        p,opp=player,3-player

        prev=None

        black_rave=[]

        white_rave=[]

        next_moves=get_valid_actions(state)

        # next_moves=set(get_valid_actions(state))

        while True:

            if not next_moves: return 50,black_rave,white_rave

            if prev:

                for nx,ny in get_neighbours(self.dim,prev):

                    if state[nx][ny]==0:

                        state[nx][ny]=p

                        if check_win(state,(nx,ny),p)[0]:

                            if p==player:

                                white_rave.append((nx,ny))

                                return 100,black_rave,white_rave

                            else:

                                black_rave.append((nx,ny))

                                return 0,black_rave,white_rave

                        state[nx][ny]=0


            swap_ind=randint(0,len(next_moves)-1)

            next_moves[-1],next_moves[swap_ind]=next_moves[swap_ind],next_moves[-1]

            a,b=next_moves.pop()

            move=(a,b)

            state[a][b]=p

            if p==player: white_rave.append(move)

            else: black_rave.append(move)

            p=3-p

            prev=move


            # neighbours=get_neighbours(self.dim,prev) if prev else []

            # neighbours=[i for i in neighbours if i in next_moves]

            # if neighbours: move=choice(neighbours)

            # else:

            #     if not next_moves: return 0,black_rave,white_rave

            #     else: move=choice(list(next_moves))

            # next_moves.remove(move)

            # a,b=move

            # state[a][b]=p

            # if p==self.player_number: white_rave.append(move)

            # else: black_rave.append(move)

            # if check_win(state,move,player)[0]: return 100,black_rave,white_rave

            # elif check_win(state,move,opp)[0]: return -100,black_rave,white_rave

            # p=3-p

            # prev=move


    def back_propogate(self,node:Node,player:int,outcome:int,black_rave:list[Tuple[int,int]],white_rave:list[Tuple[int,int]])->None:

        while node:

            if self.player_number==node.player_number:

                for point in white_rave:

                    if point in node.children:

                        node.children[point].n_rave+=1

                        node.children[point].t_rave+=-outcome

            else:

                for point in black_rave:

                    if point in node.children:

                        node.children[point].n_rave+=1

                        node.children[point].t_rave+=-outcome

            node.n+=1

            node.t+=outcome

            outcome=-outcome

            node=node.parent


    def iteration(self)->None:

        # node=self.root

        selected=self.traversal()

        if not selected.is_unexplored():

            selected=self.expansion(selected)

        outcome,black_rave,white_rave=self.rollout(deepcopy(selected.state),selected.player_number)

        self.back_propogate(selected,self.root.player_number,outcome,black_rave,white_rave)

        None


class AIPlayer:


    def __init__(self, player_number: int, timer):

        #- Run `fetch_remaining_time(timer, player_number)` to fetch remaining time of a player

        self.player_number = player_number

        self.type = 'ai'

        self.player_string = 'Player {}: ai'.format(player_number)

        self.timer = timer

        self.try_s=4

        self.confidence=0.55

        self.starters={

            3:[],

            4:[

                # [[(0,0)],[(1,2)],[(1,4)],[(0,6)],[(0,1),(1,1)],[(1,3),(2,3)],[(0,5),(1,5)]],

                # [[(0,3)],[(1,4)],[(2,5)],[(3,6)],[(1,3),(0,4)],[(2,4),(1,5)],[(3,5),(2,6)]],

                # [[(0,6)],[(2,5)],[(4,4)],[(6,3)],[(1,5),(1,6)],[(3,4),(3,5)],[(5,3),(5,4)]],

                # [[(3,6)],[(4,4)],[(4,2)],[(3,0)],[(3,5),(4,5)],[(4,3),(5,3)],[(3,1),(4,1)]],

                # [[(6,3)],[(4,2)],[(2,1)],[(0,0)],[(5,2),(5,3)],[(3,1),(3,2)],[(1,0),(1,1)]],

                # [[(3,0)],[(2,1)],[(1,2)],[(0,3)],[(2,0),(3,1)],[(1,1),(2,2)],[(0,2),(1,3)]],



                [[(0,0)],[(0,3)],[(1,2)],[(0,1),(1,1)],[(0,2),(1,3)]],

                [[(0,3)],[(0,6)],[(1,4)],[(1,3),(0,4)],[(0,5),(1,5)]],

                [[(0,6)],[(2,5)],[(3,6)],[(1,5),(1,6)],[(3,5),(2,6)]],

                [[(3,6)],[(4,4)],[(6,3)],[(3,5),(4,5)],[(5,3),(5,4)]],

                [[(6,3)],[(3,0)],[(4,2)],[(5,2),(5,3)],[(4,1),(3,1)]],

                [[(3,0)],[(2,1)],[(0,0)],[(2,0),(3,1)],[(1,0),(1,1)]]

            ],

            5:[

                [[(0,0)],[(1,2)],[(2,4)],[(1,6)],[(0,8)],[(0,1),(1,1)],[(1,3),(2,3)],[(1,5),(2,5)],[(0,7),(1,7)]],

                [[(0,4)],[(1,5)],[(2,6)],[(3,7)],[(4,8)],[(0,5),(1,4)],[(2,5),(1,6)],[(3,6),(2,7)],[(4,7),(3,8)]],

                [[(0,8)],[(2,7)],[(4,6)],[(6,5)],[(8,4)],[(1,7),(1,8)],[(3,6),(3,7)],[(5,5),(5,6)],[(7,4),(7,5)]],

                [[(4,8)],[(5,6)],[(6,4)],[(5,2)],[(4,0)],[(4,7),(5,7)],[(5,5),(6,5)],[(5,3),(6,3)],[(4,1),(5,1)]],

                [[(8,4)],[(6,3)],[(4,2)],[(2,1)],[(0,0)],[(1,0),(1,1)],[(3,1),(3,2)],[(5,2),(5,3)],[(7,3),(7,4)]],

                [[(4,0)],[(3,1)],[(2,2)],[(1,3)],[(0,4)],[(3,0),(4,1)],[(2,1),(3,2)],[(1,2),(2,3)],[(0,3),(1,4)]],

            ],

            6:[

                [[(0,0)],[(1,2)],[(0,3)],[(0,4)],[(0,5)],[(0,1),(1,1)],[(0,2),(1,3)]],

                [[(0,0)],[(1,2)],[(0,3)],[(1,5)],[(0,5)],[(0,1),(1,1)],[(0,2),(1,3)],[(0,4),(1,4)]],

                [[(0,5)],[(1,6)],[(0,8)],[(0,9)],[(0,10)],[(0,6),(1,5)],[(0,7),(1,7)]],

                [[(0,5)],[(1,6)],[(0,8)],[(1,9)],[(0,10)],[(0,6),(1,5)],[(0,7),(1,7)],[(1,8),(0,9)]],

                [[(0,10)],[(2,9)],[(3,10)],[(4,10)],[(5,10)],[(1,9),(1,10)],[(3,9),(2,10)]],

                [[(0,10)],[(2,9)],[(3,10)],[(5,9)],[(5,10)],[(1,9),(1,10)],[(3,9),(2,10)],[(4,9),(4,10)]],

                [[(5,10)],[(6,8)],[(8,7)],[(9,6)],[(10,5)],[(5,9),(6,9)],[(7,7),(7,8)]],

                [[(5,10)],[(6,8)],[(8,7)],[(9,5)],[(10,5)],[(5,9),(6,9)],[(7,7),(7,8)],[(8,6),(9,6)]],

                [[(10,5)],[(8,4)],[(7,2)],[(6,1)],[(5,0)],[(7,3),(8,3)],[(9,4),(9,5)]],

                [[(10,5)],[(8,4)],[(7,2)],[(5,1)],[(5,0)],[(7,3),(8,3)],[(9,4),(9,5)],[(6,1),(6,2)]],

                [[(5,0)],[(4,1)],[(2,0)],[(1,0)],[(0,0)],[(3,0),(3,1)],[(4,0),(4,1)]],

                [[(5,0)],[(4,1)],[(2,0)],[(1,1)],[(0,0)],[(3,0),(3,1)],[(4,0),(4,1)],[(1,0),(1,1)]]

            ],

            8:[]

        }

        for matrix in self.starters.values(): shuffle(matrix)


    def get_move(self, state: np.array) -> Tuple[int, int]:

        self.dim=int((1+len(state))/2)

        self.try_s-=1

        self.confidence+=0.07

        tree=MCTS(state,self.player_number,self.starters[self.dim],self.try_s,self.confidence)

        # iters=int(fetch_remaining_time(self.timer,self.player_number)*9)

        iters=1500 if self.try_s>-4 else int(fetch_remaining_time(self.timer,self.player_number)*9)

        # print("Iterations are : ",iters)

        return tree.choose(state,iters)


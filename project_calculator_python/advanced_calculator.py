from simple_calculator import SimpleCalculator
from stack import Stack

class AdvancedCalculator(SimpleCalculator):
	def __init__(self):
		self.history = []
		super().__init__()

	def evaluate_expression(self, input_expression):
		tok = self.tokenize(input_expression)
		if self.check_brackets(tok):

			res = self.evaluate_list_tokens(tok)
			self.history.insert(0, (input_expression, res))
			return res
		else:
			self.history.insert(0, (input_expression, 'Error'))
			return 'Error'

	def tokenize(self, input_expression):
		list = input_expression.split()
		lis2 = []
		sym = ['-','/','*','+','(',')','{','}']
		for i in list:
			if i.isdigit():
				lis2.append(int(i))
			elif i in sym:
				lis2.append(i)
			else:
				temp = ''
				for j in range(len(i)):

					if i[j] in sym and len(temp) ==0:
						lis2.append(i[j])
					elif i[j] in sym and len(temp) != 0:
						lis2.append(int(temp))
						temp = ''
						lis2.append(i[j])
					elif i[j].isdigit():
						temp += i[j]
				if temp:
					lis2.append(int(temp))
		return lis2
	def check_brackets(self, list_tokens):
		opn = ['{','(']
		clos = ['}',')']
		list = []
		for ku in list_tokens:
			if ku in opn or ku in clos:
				list.append(ku)
		bracket = []
		for i in list:
			if i in opn:
				if i == '{' and len(bracket)>0 and bracket[-1] == '(':
					return False
				bracket.append(i)
			elif i in clos:
				idx = clos.index(i)
				if len(bracket)>0 and bracket[-1] == opn[idx]:
					bracket.pop()
				else:
					bracket.append(i)

		if not bracket:
			return True
		else:
			return False

	def Bodmas(self, string):
		try:
			dustbin = []
			temp = ''
			for i in string:
				if i.isdigit() or i == '.':
					temp += i
				elif i in ['+', '-']:
					if not temp:
						temp += i
					else:
						if temp[-1].isdigit():
							dustbin.append(float(temp))
							temp = i
						elif temp[-1] == 'e':
							temp += i
						elif temp[-1] == i:
							temp = '+'
						else:
							temp = '-'
				elif i == 'e':
					temp+=i
				else:
					dustbin.append(float(temp))
					dustbin.append(i)
					temp = ''

			dustbin.append(float(temp))
			cod = True
			while cod:
				cod = False
				for j in range(len(dustbin)):
					if dustbin[j] == '/':
						t = dustbin[j - 1] / dustbin[j + 1]
						dustbin.pop(j)
						dustbin.pop(j)
						dustbin.pop(j - 1)
						dustbin.insert(j - 1, t)
						cod = True
						break
			cod = True
			while cod:
				cod = False
				for j in range(len(dustbin)):
					if dustbin[j] == '*':
						t = dustbin[j - 1] * dustbin[j + 1]
						dustbin.pop(j)
						dustbin.pop(j)
						dustbin.pop(j - 1)
						dustbin.insert(j - 1, t)
						cod = True
						break
			return sum(dustbin)
		except:
			return 'Error'



	def evaluate_list_tokens(self, list_tokens):
		try:
			stack_brackets = Stack()
			bod = ''
			temp = ''
			for i in range(len(list_tokens)):
				c = list_tokens[i]
				if type(list_tokens[i]) == float or type(list_tokens[i]) == int:
					bod += str(list_tokens[i])
				if list_tokens[i] in ['{', '(']:
					if not bod:
						bod += c
					elif bod[-1].isdigit():
						bod += '*'+list_tokens[i]
					else:
						bod += list_tokens[i]
				if list_tokens[i] in ['*','-','+', '/']:
					bod += list_tokens[i]
				if list_tokens[i] in ['}', ')']:
					for j in bod[::-1]:
						if j in ['{','(']:
							break
						else:
							temp += j
							idx = bod.index(j)
					realtemp = temp[::-1]
					temp = ''
					for we in range(len(realtemp)+1):
						bod = bod[:-1]
					l = str(self.Bodmas(realtemp))
					bod = bod + l
			return self.Bodmas(bod)
		except:
			return 'Error'


	def get_history(self):
		return self.history



a = AdvancedCalculator()
#b = a.tokenize('((11) )* (((  11)   - (20 -  61 +26+ 87-71 / 62  )) ) {47 } *1 +{ 26/73} + 52 +53/29/(16)*37+12   - ( (89) (    19-35/34/ 74+(  (((22)/96 ) -7/65)+100  ))- (  89/(30-  75-(64))))')
#print(b)
#c = a.check_brackets(b)
#print(c)
print(a.evaluate_expression('96/23 /66/ 74 * 83 -76- 54*(19 /(54)/ 15/69-39 * (  22 ))*11 -71* 48*13-80-37/(65 /(((15*  (( 23/  35/(((70-(  23*(  (20 /30 - 65    *((96*53))))  )*76  * ( 23*74  *66))))))))))'))
print(a.check_brackets(a.tokenize('96/23 /66/ 74 * 83 -76- 54*(19 /(54)/ 15/69-39 * (  22 ))*11 -71* 48*13-80-37/(65 /(((15*  (( 23/  35/(((70-(  23*(  (20 /30 - 65    *((96*53))))  )*76  * ( 23*74  *66))))))))))')) )

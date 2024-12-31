class SimpleCalculator:
    def __init__(self):
        self.history = []

    def evaluate_expression(self, exp):

        try:
            if len(exp.split())>3:
                self.history.insert(0, (exp,'Error'))
                return 'Error'
            for i in range(len(exp)):
                if exp[i-1].isdigit() and exp[i+1].isdigit():
                    self.history.insert(0,(exp,'Error'))
                    return 'Error'
            a = True if '-' in exp else False
            a1 = 1 if a else 0
            b = True if '+' in exp else False
            b1 = 1 if b else 0
            c = True if '/' in exp else False
            c1 = 1 if c else 0
            d = True if '*' in exp else False
            d1 = 1 if d else 0
            if a1+b1+c1+d1 !=1:
                self.history.insert(0,(exp,'Error'))
                return 'Error'
            temp_exp = exp.replace(' ', '')
            if a:
                express = temp_exp.split('-') + ['-']
            elif b:
                express = temp_exp.split('+') + ['+']
            elif c:
                express = temp_exp.split('/') + ['/']
            elif d:
                express = temp_exp.split('*') + ['*']
            else:
                self.history.insert(0,(exp,'Error'))
                return 'Error'
            op1 = int(express[0])
            op2 = int(express[1])
            operation = express[2]
            if operation =="+":
                result = float(op1+op2)

            elif operation =="-":


                result = float(op1-op2)
            elif operation =="/":

                if op2 == 0:
                    self.history.insert(0, (exp, 'Error'))
                    return "Error"

                result = float(op1/op2)
            elif operation == "*":
                result = float(op1*op2)

            else:
                result = "Error"
            self.history.insert(0, (exp, result))


            return result
        except:
            self.history.insert(0,(exp, 'Error'))
            return('Error')



    def get_history(self):
        return self.history

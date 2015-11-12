# define selector class
# using strategy pattern
#

class selector:
   """Select representative number out of a list of values"""
   def __init__(self, select_method=None):
      """Initialize select class"""
      if select_method:
         self.select = select_method

   def select(self, numbers):
      print 'undifined select method'

def selectMin(numbers):
  numbers.sort()
  return numbers[0]

def selectMedian(numbers):
  numbers.sort()
  return numbers[int(len(numbers)/2)]

#def selectKBest(numbers):
  

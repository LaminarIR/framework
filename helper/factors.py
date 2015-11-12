from fractions import gcd

def lcm(a,b):
    """Compute least common multiplier of two numbers"""
    return a * b / gcd(a,b)

def listgcd(l):
    """Compute the gcd of a list of numbers"""
    if len(l) == 0:
        return 0
    elif len(l) == 1:
        return l[0]
    elif len(l) == 2:
        return gcd(l[0],l[1])
    else:
        return gcd(l[0],listgcd(l[2:]))

def listlcm(l):
    """Compute the gcd of a list of numbers"""
    if len(l) == 0:
        return 0
    elif len(l) == 1:
        return l[0]
    elif len(l) == 2:
        return lcm(l[0],l[1])
    else:
        return lcm(l[0],listlcm(l[2:]))

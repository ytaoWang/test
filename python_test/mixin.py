#!/usr/bin/env python

import types

def MixIn(pyClass,mixInClass,makeAncestor=0):

    print 'pyClass:',pyClass.__name__,',mixIn:',mixInClass.__name__

    if makeAncestor:
        pyClass.__bases__ = (mixInClass,) + pyClass.__bases__
    else:
        # Recursively traverse the mix-in ancestor
        # classes in order to support inheritance
        print 'mixInClass:',mixInClass.__bases__,',mro:',mixInClass.__mro__
        baseClasses = list(mixInClass.__bases__)
        baseClasses.reverse()

        for baseClass in baseClasses:
            MixIn(pyClass,baseClass)
        
        # Install the mix-in methods into the class
        for name in dir(mixInClass):
            if not name.startswith('__'):
                # skip private members
                member = getattr(mixInClass, name)
                if type(member) is types.MethodType:
                    member = member.im_func

                setattr(pyClass,name,member)


class C1(object):
    def test(self):
        print 'test in C1'

class C0MixIn(object):
    def test(self):
        print 'test in C0MixIn'

class C2(C1,C0MixIn):
    def test(self):
        print 'test in C2'

class C0(C2,C1):
    pass

if __name__ == "__main__":

    # C0.__bases__ = (C0MinIn,)  + C0.__bases__
    MixIn(C0,C0MixIn)
    c0 = C0()
    c0.test()
    print C0.__mro__

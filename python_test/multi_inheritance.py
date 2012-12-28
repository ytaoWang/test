#! /usr/bin/env python
# 
# multi inheritance view
#                         /------ 7-------\ 
#                        /        |        \
#                       4       - 5         6
#                         \\   /           /
#                           2-/\--- 3------/
#                            \   /    
#                             0,1     
#
#

class C7(object):
    def test(self):
        print 'test in C7'

class C4(C7):
    def test(self):
        print 'test in C4'

class C5(C7):
    def test(self):
        print 'test in C5'

class C6(C7):
    def test(self):
        print 'test in C6'

class C2(C4,C5):
    def test(self):
        print 'test in C2'

class C3(C4,C6):
    pass

class C1(C2,C3):
    pass

class C0(C2,C3):
    def test(self):
        print 'test in C0'
        super(C3,self).test()

if __name__ == "__main__":
    
    c1 = C1()
    c1.test()
    c0 = C0()
    c0.test()
    print C7.__mro__
    print C6.__mro__
    print C5.__mro__
    print C4.__mro__
    print C3.__mro__
    print C2.__mro__
    print C1.__mro__
    print C0.__mro__

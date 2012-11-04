#!/usr/bin/python


import logging

def count_lines(filename):
    """
    Count the number of lines in a file.If the file can't be opened,it should be treated
    the same as if it was empty.
    """

    try:
        file = open(filename,'r')
    except TypeError as e:
        #The filename wasn't valid for use with the filesystem
        logging.error(e)
        return 0
    except EnvironmentError as e:
        #Something went wrong reading the file.
        logging.error(e.args[1])
        return 0
    else:
        return len(file.readlines())
    finally:
        if file:
            print "close the file %s" % filename
            file.close()

def count_lines2(filename):
    """Count the number of lines in a file."""
    with open(filename,'r') as file:
        return len(file.readlines())

def echo():
    """Returns everything you type until you presss Ctrl-C"""
    
    while True:
        try:
            print(input('Type Something: '))
        except KeyboardInterrupt:
            print() #Make sure the prompt appears on a new line
            break

if __name__ == '__main__':
    print "line %s" % count_lines("pro_27.py")
    count_lines2('pro_27.py')
    echo()

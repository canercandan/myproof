#!/usr/bin/env python

import optparse, logging, sys, os
from datetime import datetime

LEVELS = {'debug': logging.DEBUG,
          'info': logging.INFO,
          'warning': logging.WARNING,
          'error': logging.ERROR,
          'critical': logging.CRITICAL}

LOG_DEFAULT_FILENAME='notitle.log'

RESULT_FILE_FORMAT='%s%s_p%d_pS%d_P%d_d%d_dS%d_D%d_r%d_s%d'

def parser(parser=optparse.OptionParser()):
    # general parameters
    parser.add_option('-v', '--verbose', choices=LEVELS.keys(), default='info', help='set a verbose level')
    parser.add_option('-o', '--output', help='give an output filename for logging', default=LOG_DEFAULT_FILENAME)
    # general parameters ends

    parser.add_option('-p', '--prefix', help='give the prefix path where binaries are', default='.')

    parser.add_option('-s', '--staticfile', help='give the static file', default='')
    parser.add_option('-d', '--dynamicfile', help='give the dynamic file', default='')

    parser.add_option('-g', '--graphic', action='store_true', default=False, help='generate graphic')
    parser.add_option('-l', '--functionslist', action='store_true', default=False, help='list all functions')

    options, args = parser.parse_args()

    logger(options.verbose, options.output)

    return options

def logger(level_name, filename=LOG_DEFAULT_FILENAME):
    logging.basicConfig(
        level=logging.DEBUG,
        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
        filename=filename, filemode='a'
        )

    console = logging.StreamHandler()
    console.setLevel(LEVELS.get(level_name, logging.NOTSET))
    console.setFormatter(logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s'))
    logging.getLogger('').addHandler(console)

options = parser()

def generate_plot( functions ):
    if not options.graphic:
        return

    import numpy as np
    import matplotlib.pyplot as plt

    def max_ninstance():
        res = 0
        for func in functions: res = func[3] if func[3] > res else res
        return res

    ninstance = max_ninstance()
    nfunction = len( functions ) # the number of function
    ind = np.arange(nfunction)  # the x locations for the instances
    width = 0.35       # the width of the bars
    plt.subplot(111)

    def get_instances():
        instances = []
        for i in range(0, ninstance):
            instance = []
            for function in functions:
                instance.append( function[4+i*2] if function[3] > i else 0 )
            instances.append(instance)
        return instances

    instances = get_instances()

    print instances

    colors = ['b','r','g','y','o']

    rectss = []
    for i in range(0, len(instances)):
        rectss.append( plt.bar( ind+(width*i), instances[i], width, color=colors[i % len(colors)] ) )

    print rectss

    # add some
    plt.ylabel('Cycles')
    plt.xlabel('Instances')
    plt.title('Cycle\'s measure by instance and function')
    plt.xticks(ind+width, tuple( [ value for value in range(0, ninstance) ] ) )

    #rects1 = plt.bar(ind, menMeans, width, color='r')

    #womenMeans = (25, 32, 34, 20, 25)
    #rects2 = plt.bar(ind+width, womenMeans, width, color='y')

    plt.legend( tuple( [ rects[0] for rects in rectss ] ), tuple( [ value for value in range(0, nfunction) ] ) )

    def autolabel(rects):
        # attach some text labels
        for rect in rects:
            height = rect.get_height()
            plt.text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height), ha='center', va='bottom')

    for rects in rectss: autolabel(rects)

    plt.show()

    return

    plt.show()

def get_data( filename ):
    try:
        f = open( filename )
        return [ line.split() for line in f.readlines() ]
    except:
        raise ValueError('got an issue during the reading of file %s' % filename)

def main():
    if options.staticfile == '' or options.dynamicfile == '':
        logging.error('you have to define the existing static and dynamic files')
        return

    cmd = 'cat %s %s | %s/profiler' % (options.staticfile, options.dynamicfile, options.prefix)

    print 'cmd', cmd

    os.system( cmd )

    return

    functions = get_data(options.file)

    if options.functionslist:
        fnames = [ line[0] for line in functions ]
        if len(fnames) > 0:
            print '** List of functions **'
            for fname in fnames: print '\t', fname
            return

    # stats

    log = logging.getLogger('stats')

    for function in functions:
        if len(function) < 6:
            log.error('incorrect number of parameters \"%s\"' % function)
            return

        function[1:] = [ int(value) for value in function[1:] ]

        print '* function %s *' % function[0]
        print ' - data = { nload: %2d, nstore: %2d, ninstance: %2d }' % tuple(function[1:4])

        for i in range(0, function[3]): print ' - instance %2d = { inclusive: %2d, exclusive: %2d }' % (i, function[4+i*2], function[4+i*2+1])

    generate_plot( functions )

# when executed, just run main():
if __name__ == '__main__':
    logging.debug('### myproof started ###')

    main()

    logging.debug('### myproof ended ###')

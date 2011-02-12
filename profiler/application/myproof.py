#!/usr/bin/env python

#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# Authors:
# Caner Candan <caner@candan.fr>, http://caner.candan.fr
# Aurèle Mahéo <aurele.maheo@gmail.com>
#

import optparse, logging, sys, os
from datetime import datetime
from pprint import pprint

LEVELS = {'debug': logging.DEBUG,
          'info': logging.INFO,
          'warning': logging.WARNING,
          'error': logging.ERROR,
          'critical': logging.CRITICAL}

LOG_DEFAULT_FILENAME = 'notitle.log'

RESULT_FILE_FORMAT = '%s%s_p%d_pS%d_P%d_d%d_dS%d_D%d_r%d_s%d'

CG_FILENAME             = 'profiler_callgraph.txt'
INSTANCES_FILENAME      = 'profiler_instances.txt'
STATS_FILENAME          = 'profiler_stats.txt'
OUTPUT_FILENAME         = 'profiler_output.txt'

def parser(parser=optparse.OptionParser()):
    # general parameters
    parser.add_option('-v', '--verbose', choices=LEVELS.keys(), default='info', help='set a verbose level')
    parser.add_option('-o', '--output', help='give an output filename for logging', default=LOG_DEFAULT_FILENAME)
    # general parameters ends

    parser.add_option('-p', '--prefix', help='give the prefix path where binaries are', default='.')

    parser.add_option('-s', '--staticfile', help='give the static file', default='')
    parser.add_option('-d', '--dynamicfile', help='give the dynamic file', default='')

    parser.add_option('-S', '--summary', action='store_true', default=False, help='get the summary')
    parser.add_option('-c', '--compare', action='store_true', default=False, help='generate graphic to compare functions')
    parser.add_option('-g', '--instancesgraph', action='store_true', default=False, help='get the function instances graph')
    parser.add_option('-G', '--functionsgraph', action='store_true', default=False, help='get the graph by functions')

    parser.add_option('-i', '--inclusive', action='store_true', default=True, help='get the graph with inclusive data')

    parser.add_option('-e', '--exclusive', action='store_true', default=False, help='get the graph with exclusive data')

    parser.add_option('-l', '--functionslist', action='store_true', default=False, help='list all functions')

    parser.add_option('-f', '--function', help='select a function among all available', default='')

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

def max_ninstance(functions):
    res = 0
    for func in functions: res = func[3] if func[3] > res else res
    return res

def graph_function_compare( functions ):
    if not options.compare:
        return

    log = logging.getLogger('graph_function_compare')

    if len(functions) > 4:
        log.error('you have more than 4 functions')
        return

    import numpy as np
    import matplotlib.pyplot as plt

    ninstance = max_ninstance(functions)
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

def instances_graph(functions):
    if not options.instancesgraph:
        return

    log = logging.getLogger('graph_function')

    if options.function == '':
        log.error('you have to select one function among all available, see option -l')
        return

    data = None
    for function in functions:
        if options.function in function[0]:
            data = function
            break

    if data == None:
        log.error('no function found')
        return

    pprint(data)

    fname, nload, nstore, ninstance = data[0:4]

    import matplotlib.pyplot as plt

    p1, = plt.plot(range(1,ninstance+1), data[4::2], 'r', label='inclusive')
    p2, = plt.plot(range(1,ninstance+1), data[5::2], 'g', label='exclusive')

    l1 = plt.legend([p1], ["Inclusive"], loc=1)
    l2 = plt.legend([p2], ["Exclusive"], loc=4) # this removes l1 from the axes.

    plt.gca().add_artist(l1)

    plt.ylabel('cycles')
    plt.xlabel('instances')
    plt.show()

def functions_graph(functions):
    if not options.functionsgraph:
        return

    log = logging.getLogger('functions_graph')

    import matplotlib.pyplot as plt

    data = None

    if options.exclusive:
        plt.title('exclusive time')
        data = [ function[5::2] for function in functions ]
    else:
        plt.title('inclusive time')
        data = [ function[4::2] for function in functions ]

    plt.xlabel('functions')
    plt.ylabel('cycles')

    plt.boxplot( data )

    plt.show()

def get_data( filename ):
    try:
        f = open( filename )
        return [ line.split() for line in f.readlines() ]
    except:
        raise ValueError('got an issue during the reading of file %s' % filename)

def list_functions(functions):
    if options.functionslist:
        fnames = [ line[0] for line in functions ]
        if len(fnames) > 0:
            print '** List of functions **'
            for fname in fnames: print '\t', fname
            sys.exit()

def check_errors(functions):
    log = logging.getLogger('check_errors')

    for function in functions:
        if len(function) < 6:
            log.error('incorrect number of parameters \"%s\"' % function)
            sys.exit()

def format_functions(functions):
    for function in functions:
        function[1:] = [ int(value) for value in function[1:] ]

def get_function_summary(function):
    print '* function %s *' % function[0]
    print ' - data = { nload: %2d, nstore: %2d, ninstance: %2d }' % tuple(function[1:4])

    for i in range(0, function[3]): print ' - instance %2d = { inclusive: %2d, exclusive: %2d }' % (i, function[4+i*2], function[4+i*2+1])

def get_summary(functions):
    if not options.summary: return

    for function in functions:
        if options.function in function[0]:
            get_function_summary(function)

def main():
    if options.staticfile == '' or options.dynamicfile == '':
        logging.error('you have to define the existing static and dynamic files')
        return

    cmd = 'cat %s %s | %s/profiler' % (options.staticfile, options.dynamicfile, options.prefix)

    logging.info('executed profiler command: %s' % cmd)

    os.system( cmd )

    functions = get_data(INSTANCES_FILENAME)
    #functions = get_data('test/test.txt')

    list_functions(functions)
    check_errors(functions)
    format_functions(functions)
    get_summary(functions)
    instances_graph(functions)
    functions_graph(functions)
    graph_function_compare(functions)

# when executed, just run main():
if __name__ == '__main__':
    logging.debug('### myproof started ###')
    main()
    logging.debug('### myproof ended ###')

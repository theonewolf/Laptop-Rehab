#!/usr/bin/env python
###############################################################################
# laptop_csv_to_html.py                                                       #
#                                                                             #
# Convert CSV files exported from Google Drive into HTML based on a template. #
#                                                                             #
#                                                                             #
#   Authors: Wolfgang Richter <wolf@cs.cmu.edu>                               #
#                                                                             #
#   Copyright 2013 Laptop Rehab Project                                       #
#                                                                             #
# Permission is hereby granted, free of charge, to any person obtaining a     #
# copy of this software and associated documentation files (the "Software"),  #
# to deal in the Software without restriction, including without limitation   #
# the rights to use, copy, modify, merge, publish, distribute, sublicense,    #
# and/or sell copies of the Software, and to permit persons to whom the       #
# Software is furnished to do so, subject to the following conditions:        #
#                                                                             #
#     The above copyright notice and this permission notice shall be included #
#     in all copies or substantial portions of the Software.                  #
#                                                                             #
#     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS #
#     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              #
#     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  #
#     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    #
#     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    #
#     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       #
#     SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  #
#                                                                             #
###############################################################################



# Imports
from collections import OrderedDict
from csv import reader as csvreader
from jinja2 import Template
from sys import argv



# Globals
HELP = '''Usage: %s <TEMPLATE> <CSV File(s) ...>'''
OUTFNAME_TEMPLATE = '''%d.html'''
EMAIL = 'laptop-rehab@lists.andrew.cmu.edu'



# main()
if __name__ == '__main__':

    if len(argv) < 3:
        print HELP % (argv[0])
        exit(1)

    template = argv[1]
    csv_files = argv[2:]

    print '-- Converting CSV file to templatized HTML --'
    print '-- Using template: %s' % (template)
    print '-- Operating on CSV files: %s' % (csv_files)

    counter = 0

    with open(template, 'rb') as template:
        template = template.read()
        template = Template(template)
        for f in csv_files:
            with open(f, 'rb') as csvfile:
                csvdata = csvreader(csvfile)
                keys = None

                for row in csvdata:
                    if not keys: keys = row; continue

                    stats = OrderedDict(zip(keys,row))
                    laptop_name = '%s %s' % (stats['Manufacturer'],
                                             stats['Model #'])
                    OS = row[keys.index('Installed OS')] 
                    username,password = stats['Username-Password'].split('-')

                    del stats['Notes']
                    del stats['Username-Password']

                    with open(OUTFNAME_TEMPLATE % (counter), 'wb') as out:
                        out.write(template.render(laptop_name=laptop_name,
                                                  OS=OS,
                                                  email=EMAIL,
                                                  username=username,
                                                  password=password,
                                                  stats=stats))
                        counter += 1

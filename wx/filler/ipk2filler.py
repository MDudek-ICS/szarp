#!/usr/bin/env python
# -*- coding: ISO-8859-2 -*-
# SZARP: SCADA software 
# 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

# Gets editable params from params.xml and changes theirs *szb database
# files access permissions to 'a+rw'
#
# Uses functions form filler.py

import os
import sys
sys.path.append('/opt/szarp/lib/python')
from filler import readParamsFromFile
from filler import mkPath

# changes parameter name to path
def paramToPath(p):
	p = p.replace(':','/')
	p = p.replace(' ','_')
	p = p.replace(u'�','a')
	p = p.replace(u'�','c')
	p = p.replace(u'�','e')
	p = p.replace(u'�','l')
	p = p.replace(u'�','n')
	p = p.replace(u'�','s')
	p = p.replace(u'�','o')
	p = p.replace(u'�','z')
	p = p.replace(u'�','z')
	p = p.replace(u'�','A')
	p = p.replace(u'�','C')
	p = p.replace(u'�','E')
	p = p.replace(u'�','L')
	p = p.replace(u'�','N')
	p = p.replace(u'�','S')
	p = p.replace(u'�','O')
	p = p.replace(u'�','Z')
	p = p.replace(u'�','Z')
	return p


if len(sys.argv) == 2:
	if sys.argv[1] == '--help':
		print 'Usage: ipk2filler [PATH]'
		print 'Gives write permission to all editable params dirs.'
		print
		print 'PATH	Path to params.xml file. If ommited working directory is taken.'
		sys.exit(1)
	else:
		confDir = sys.argv[1]
elif len(sys.argv) == 1:
	confDir = "."
else:
	print 'Usage: ipk2filler [PATH]'



try:
	# gets path to params.xml
	xmlFile=file(confDir+"/params.xml")
except IOError:
	print 'No',confDir+"/params.xml file. Check if given path is correct."
	sys.exit(2)

# gets database dir
path=confDir+"/../szbase"

# get list of editable params
params=readParamsFromFile(xmlFile)

for p in params:
	pPath=path+'/'+paramToPath(p)
	if not os.access(pPath,os.F_OK):
		mkPath(pPath)
	os.system('chmod -R a+rwx '+pPath)

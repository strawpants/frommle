# This file is part of Frommle
# Frommle is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.

# Frommle is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with Frommle; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# Author Roelof Rietbroek (roelof@wobbly.earth), 2019
import yaml
import os

def usersettings():
    """Retrieves the dictionary with settings from the .frommle.yaml file in the user's home"""
    settingsFile=os.path.join(os.path.expanduser('~'),'.frommle.yaml')
    #read last used settings
    if os.path.exists(settingsFile):
        #Read parameters from yaml file
        with open(settingsFile, 'r') as fid:
            settings=yaml.safe_load(fid)
    else:
        settings={}

    return settings

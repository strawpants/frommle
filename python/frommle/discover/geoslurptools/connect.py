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
from geoslurp.db.connector import GeoslurpConnector
from frommle.core.usersettings import usersettings
import keyring
def connect(dbname=None):
    """connects to a geoslurp database and returns a connector object"""
    #find default settings from the configuration file
    settings=usersettings()

    if not "Defaultdb" in settings:
        raise ValueError("Don't know which database to connect to")

    if not dbname:
        dbname=settings["Defaultdb"]

    dbentry=settings[dbname]

    #retrieve password
    if settings["Authstore"] == "libsecret":
        passwd=keyring.get_password(dbname,"roelof")

    else:
        passwd=settings["Secrets"][dbname]["passwd"]
    
    return GeoslurpConnector(dbentry["host"],dbentry["user"],passwd)


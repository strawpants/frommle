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

# Author Roelof Rietbroek (roelof@geod.uni-bonn.de), 2019

from sqlalchemy.sql import text
from sqlalchemy import select

def uriMatch(dbcon,uriregex,table,scheme="public"):
    tbl=dbcon.getTable(table,scheme)
    ses=dbcon.Session()
    qry=select([tbl]).where(text("uri ~ :reg ").bindparams(reg=uriregex))
    return dbcon.dbeng.execute(qry)



def nameMatch(dbcon,nameregex,table,scheme="public"):
    tbl=dbcon.getTable(table,scheme)
    qry=select([tbl]).where(text("name ~ :reg ").bindparams(reg=nameregex))
    return dbcon.dbeng.execute(qry)


def nameEquals(dbcon,name,table,scheme="public"):
    tbl=dbcon.getTable(table,scheme)
    qry=select([tbl]).where(text("name = :reg ").bindparams(reg=name))
    return dbcon.dbeng.execute(qry)

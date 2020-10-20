from frommle.core import V_float64,V_size_t
import numpy as np

def makeVec(listlike):
    """factory method to create an exchangable boot::python vector wrapper from a listlike object"""
    tp=type(listlike[0])
    if tp == float or tp == np.float64:
        v=V_float64()
    elif type(listlike[0])  == int:
        v=V_size_t()
    else:
        raise RuntimeError("unknown list type %s, please extend makeVec in core/vector.py"% str(type(listlike[0])))
    
    # v.reserve(len(listlike))
    for val in listlike:
        v.append(val)

    return v


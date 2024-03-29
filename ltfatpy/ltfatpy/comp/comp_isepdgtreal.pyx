# -*- coding: utf-8 -*-
# ######### COPYRIGHT #########
# Credits
# #######
#
# Copyright(c) 2015-2018
# ----------------------
#
# * `LabEx Archimède <http://labex-archimede.univ-amu.fr/>`_
# * `Laboratoire d'Informatique Fondamentale <http://www.lif.univ-mrs.fr/>`_
#   (now `Laboratoire d'Informatique et Systèmes <http://www.lis-lab.fr/>`_)
# * `Institut de Mathématiques de Marseille <http://www.i2m.univ-amu.fr/>`_
# * `Université d'Aix-Marseille <http://www.univ-amu.fr/>`_
#
# This software is a port from LTFAT 2.1.0 :
# Copyright (C) 2005-2018 Peter L. Soendergaard <peter@sonderport.dk>.
#
# Contributors
# ------------
#
# * Denis Arrivault <contact.dev_AT_lis-lab.fr>
# * Florent Jaillet <contact.dev_AT_lis-lab.fr>
#
# Description
# -----------
#
# ltfatpy is a partial Python port of the
# `Large Time/Frequency Analysis Toolbox <http://ltfat.sourceforge.net/>`_,
# a MATLAB®/Octave toolbox for working with time-frequency analysis and
# synthesis.
#
# Version
# -------
#
# * ltfatpy version = 1.0.16
# * LTFAT version = 2.1.0
#
# Licence
# -------
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ######### COPYRIGHT #########


"""This module contains interface functions for the LTFAT computed
versions of isepdgtreal calculations.

.. moduleauthor:: Denis Arrivault
"""

from __future__ import print_function, division

import cython
import numpy as np

from ltfat cimport ltfatInt, dgt_phasetype, idgtreal_fb_d, idgtreal_long_d
from ltfatpy.comp.ltfat cimport TIMEINV, FREQINV

# don’t check for out-of-bounds indexing.
@cython.boundscheck(False)
# assume no negative indexing.
@cython.wraparound(False)
cdef comp_idgtreal_long_d(const double complex[:] coef, const double[:] g,
                          const int L, const int W, const int a, const int M,
                          const dgt_phasetype ptype, double[:] out):
    """ Internal function, do not use it """
    idgtreal_long_d(&coef[0], &g[0], L, W, a, M, ptype, &out[0])

# don’t check for out-of-bounds indexing.
@cython.boundscheck(False)
# assume no negative indexing.
@cython.wraparound(False)
cdef comp_idgtreal_fb_d(const double complex[:] coef, const double[:] g,
                        const int L, const int gl, const int W, const int a,
                        const int M, const dgt_phasetype ptype, double[:] out):
    """ Internal function, do not use it """
    idgtreal_fb_d(&coef[0], &g[0], L, gl, W, a, M, ptype, &out[0])

# don’t check for out-of-bounds indexing.
@cython.boundscheck(False)
# assume no negative indexing.
@cython.wraparound(False)
cpdef comp_isepdgtreal(coef, g, a, M, pt):
    """Function that computes separable inverse real dgt

    This is a computational subroutine, do not call it directly, use
    :func:`~ltfatpy.gabor.idgtreal.idgtreal` instead.

    .. seealso:: :func:`~ltfatpy.gabor.idgt.idgt`
    """

    if (coef.dtype.type != np.complex128):
        raise TypeError("coef data should be numpy.complex128")
    if (g.dtype.type != np.float64):
        raise TypeError("g data should be numpy.float64")
    if pt != 0 and pt != 1:
        raise TypeError("pt should be 0 (FREQINV) or 1 (TIMEINV)")
    if coef.ndim <= 1 or coef.ndim > 3:
        raise TypeError("Dimensions of dgt coefficients array must be 2 or 3.")
    cdef ltfatInt N = coef.shape[1]
    cdef ltfatInt L = N * a
    cdef ltfatInt M2 = coef.shape[0]
    if M2 != (M//2 + 1):
        raise TypeError("Mismatch between the specified number of channels " +
                        "and the size of the input coefficients.")
    cdef ltfatInt W = coef.size // (M2 * N)
    if W > 1 and coef.ndim == 2:
        raise TypeError("Dimensions of dgt coefficients array do not fit.")
    if W == 1 and coef.ndim == 3:
        coef = np.squeeze(coef, axis=2)

    coef_combined = coef.reshape(coef.size, order='F')

    cdef ltfatInt gl = g.shape[0]

    if g.ndim > 1:
        if g.ndim > 2:
            g = np.squeeze(g, axis=range(2, g.ndim-1))
        if g.ndim == 2:
            gl = gl * g.shape[1]
        g_combined = g.reshape(gl, order='F')
    else:
        g_combined = g
    res = np.ndarray((L * W), dtype=np.float64)
    if gl < L:
        comp_idgtreal_fb_d(coef_combined, g_combined, L, gl, W, a, M, pt, res)
        if W > 1:
            return np.reshape(res, (L, W), order='F')
        return res
    comp_idgtreal_long_d(coef_combined, g_combined, L, W, a, M, pt, res)
    if W > 1:
        return np.reshape(res, (L, W), order='F')
    return res

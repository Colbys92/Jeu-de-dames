#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


arbre_module = Extension('_arbre',
                           sources=['arbre_wrap.cxx','arbre.cpp'],
                           )

setup (name = 'arbre',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Piece and Board structures""",
       ext_modules = [arbre_module],
       py_modules = ["arbre"],
       )
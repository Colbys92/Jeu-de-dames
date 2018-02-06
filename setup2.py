#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


algogenetique_module = Extension('_algogenetique',
                           sources=['arbre_wrap.cxx','arbre.cpp','algogenetique_wrap.cxx','algogenetique.cpp'],
                           )

setup (name = 'algogenetique',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Individus""",
       ext_modules = [algogenetique_module],
       py_modules = ["algogenetique"],
       )
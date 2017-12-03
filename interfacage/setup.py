#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


tree_module = Extension('_tree',
                           sources=['tree_wrap.cxx'],
                           )

setup (name = 'tree',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Int Tree structure""",
       ext_modules = [tree_module],
       py_modules = ["tree"],
       )
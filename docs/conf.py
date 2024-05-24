#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# The Mesa 3D Graphics Library documentation build configuration file, created by
# sphinx-quickstart on Wed Mar 29 14:08:51 2017.
#
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys

from hawkmoth.util import compiler

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
sys.path.append(os.path.abspath('_exts'))


# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'bootstrap',
    'formatting',
    'hawkmoth',
    'nir',
    'redirects',
    'sphinx.ext.graphviz',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = 'The Mesa 3D Graphics Library'
copyright = '1995-2018, Brian Paul'
author = 'Brian Paul'
html_show_copyright = False

html_theme_path = ['.']

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = 'latest'
# The full version, including alpha/beta/rc tags.
release = 'latest'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = 'en'

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = ['header-stubs']

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False

# Disable highlighting unless a language is specified, otherwise we'll get
# python keywords highlit in literal blocks.
highlight_language = 'none'

default_role = 'c:expr'

# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'mesa3d_theme'

html_favicon = 'favicon.ico'

html_copy_source = False

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = [
  '_static/',
  'release-maintainers-keys.asc',
  'features.txt',
  'libGL.txt',
  'README.UVD',
  'README.VCE',
]

html_extra_path = []

html_redirects = [
  ('webmaster', 'https://www.mesa3d.org/website/'),
  ('developers', 'https://www.mesa3d.org/developers/'),
  ('thanks', 'https://gitlab.freedesktop.org/mesa/mesa/-/blob/amber/docs/thanks.rst'),
]


# -- Options for linkcheck ------------------------------------------------

linkcheck_ignore = [
  r'specs/.*\.spec', # gets copied during the build process
  r'news:.*', # seems linkcheck doesn't like the news: URI-scheme...
  r'http://mesa-ci-results.jf.intel.com', # only available for Intel employees
  r'https://gitlab.com/.*#.*', # needs JS eval
  r'https://gitlab.freedesktop.org/.*#.*', # needs JS eval
  r'https://github.com/.*#.*', # needs JS eval
]
linkcheck_exclude_documents = [r'relnotes/.*']

linkcheck_allowed_redirects = {
    # Pages that forward the front-page to a wiki or some explore-page
    'https://www.freedesktop.org': 'https://www.freedesktop.org/wiki/',
    'https://x.org': 'https://x.org/wiki/',
    'https://perf.wiki.kernel.org/': 'https://perf.wiki.kernel.org/index.php/Main_Page',
    'https://dri.freedesktop.org/': 'https://dri.freedesktop.org/wiki/',
    'https://gitlab.freedesktop.org/': 'https://gitlab.freedesktop.org/explore/groups',
    'https://www.sphinx-doc.org/': 'https://www.sphinx-doc.org/en/master/',

    # Pages that requires authentication
    'https://gitlab.freedesktop.org/admin/runners': 'https://gitlab.freedesktop.org/users/sign_in',
    'https://gitlab.freedesktop.org/profile/personal_access_tokens': 'https://gitlab.freedesktop.org/users/sign_in',
}


# -- Options for HTMLHelp output ------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'TheMesa3DGraphicsLibrarydoc'


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'TheMesa3DGraphicsLibrary.tex', 'The Mesa 3D Graphics Library Documentation',
     'Brian Paul', 'manual'),
]


# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'themesa3dgraphicslibrary', 'The Mesa 3D Graphics Library Documentation',
     [author], 1)
]


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'TheMesa3DGraphicsLibrary', 'The Mesa 3D Graphics Library Documentation',
     author, 'TheMesa3DGraphicsLibrary', 'One line description of project.',
     'Miscellaneous'),
]

# -- Options for Graphviz -------------------------------------------------

graphviz_output_format = 'svg'

# -- Options for hawkmoth -------------------------------------------------

hawkmoth_root = os.path.abspath('..')
hawkmoth_clang = [
  '-Idocs/header-stubs/',
  '-Iinclude/',
  '-Isrc/',
  '-Isrc/gallium/include/',
  '-Isrc/intel/',
  '-Isrc/mesa/',
  '-DHAVE_STRUCT_TIMESPEC',
  '-DHAVE_PTHREAD',
  '-DHAVE_ENDIAN_H',
]
hawkmoth_clang.extend(compiler.get_include_args())

# helpers for definining parameter direction
rst_prolog = '''
.. |in| replace:: **[in]**
.. |out| replace:: **[out]**
.. |inout| replace:: **[inout]**
'''

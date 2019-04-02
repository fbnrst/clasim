from distutils.core import setup, Extension
import os

def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


module1 = Extension('clasim.cclasim',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    language = "c++",
                    extra_compile_args=['-std=c++11'],
                    libraries = ['gsl','gslcblas'],
                    sources = ['src/cclasim.cpp',
                        'src/types.cpp',
                        'src/cell.cpp',
                        'src/rand.cpp'])

setup (name = 'clasim',
       version = '1.0',
       description = 'Clasim for labelling assays',
       packages = ['clasim'],
       author = '',
       author_email = '',
       url = '',
       long_description=read('README.md'),
       ext_modules = [module1],
       )

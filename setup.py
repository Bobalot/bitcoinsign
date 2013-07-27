from distutils.core import setup, Extension
 
module1 = Extension('bitcoinsign', sources = ['libsign.cpp'], libraries = ['crypto'])
 
setup (name = 'Bitcoin Sign',
        version = '0.1',
        description = 'Signs bitcoin messages.',
        ext_modules = [module1])


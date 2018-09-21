
def FlagsForFile(filename, **kwargs):

    flags = [
        '-D__AVR_ATtiny10__',
        '-I.',
        '-I/usr/lib/avr/include',
        '-xc',
        '-std=c99',
        # '-nostdlib',
        # Existing Warning flags
        '-Wpointer-arith',
        '-Wundef',
        '-Wall',
        '-Wextra',
        '-Werror'
        '-pedantic',
        # '-xc++',
        # '-std=c++11',
    ]

    # data = kwargs['client_data']
    # filetype = data['&filetype']

    # if filetype == 'c':
    #     flags += ['-xc', '-std=c99']
    # elif filetype == 'cpp':
    #     flags += ['-xc++']
    #     flags += ['-std=c++0x']

    return {
        'flags':    flags,
        'do_cache': True
    }

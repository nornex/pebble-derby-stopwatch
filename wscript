
#
# This file is the default set of rules to compile a Pebble project.
#
# Feel free to customize this to your needs.
#

import os.path
from pprint import pprint

top = '.'
out = 'build'
CROSS_COMPILE_PREFIX = 'arm-none-eabi-'

def options(ctx):
    ctx.load('pebble_sdk')
    ctx.load('g++')

def configure(ctx):
    ctx.load('pebble_sdk')
    ctx.load('g++')

    #print '\n-----------------------------------------------------------------------'
    #print 'CPP Default environment'
    #print '\n' + str(ctx.env)

    base_env = dict(ctx.env)

    for env in [ctx.env] + ctx.all_envs.values():
        for key, val in base_env.iteritems():
            if key.startswith('CXX'):
                env[key] = val

        env.CXX = CROSS_COMPILE_PREFIX + 'g++'

        env.CXXFLAGS = list(base_env['CFLAGS'])
        env.CXXFLAGS.remove('-std=c99')
        env.CXXFLAGS.extend(['-std=c++11', '-fPIE', '-fno-unwind-tables', '-fno-exceptions'])

def build(ctx):
    ctx.load('pebble_sdk')
    binaries = []

    for p in ctx.env.TARGET_PLATFORMS:

        ctx.set_env(ctx.all_envs[p])
        #print '\n-----------------------------------------------------------------------'
        #print '%s' % p
        #print '\n' + str(ctx.env)
        #print '\n' + str(ctx.all_envs[p])

        ctx.set_group(ctx.env.PLATFORM_NAME)
        app_elf='{}/pebble-app.elf'.format(ctx.env.BUILD_DIR)
        ctx.pbl_program(source=ctx.path.ant_glob('src/**/*.cpp'), target=app_elf)

        binaries.append({'platform': p, 'app_elf': app_elf})

    ctx.pbl_bundle(binaries=binaries, js=ctx.path.ant_glob('src/js/**/*.js'))

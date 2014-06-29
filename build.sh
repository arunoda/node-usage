#!/bin/sh

# prevent binary build on mac

PLATFORM=`node -e "console.log(process.platform);"`
if [ $PLATFORM != 'darwin' ]
then
  ./node_modules/.bin/node-gyp rebuild
fi

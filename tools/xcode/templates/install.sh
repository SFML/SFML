#!/bin/sh

# Install SFML templates for Xcode 4 into /Library/Developer/Xcode/Templates/

mkdir -p /Library/Developer/Xcode/Templates/
cp -R "$(dirname $0)/SFML" /Library/Developer/Xcode/Templates/

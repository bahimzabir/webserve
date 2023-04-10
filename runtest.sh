#!/bin/bash

# Create directory YoupiBanane
mkdir YoupiBanane

# Create files inside YoupiBanane
touch YoupiBanane/youpi.bad_extension
touch YoupiBanane/youpi.bla

# Create directory nop inside YoupiBanane
mkdir YoupiBanane/nop

# Create files inside nop directory
touch YoupiBanane/nop/youpi.bad_extension
touch YoupiBanane/nop/other.pouic

# Create directory Yeah inside YoupiBanane
mkdir YoupiBanane/Yeah

# Create file inside Yeah directory
touch YoupiBanane/Yeah/not_happy.bad_extension

# Display success message
echo "Directory YoupiBanane created with files and sub-directories."
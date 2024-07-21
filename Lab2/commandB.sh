#!/bin/sh

groupadd penguins
sudo useradd --create-home tux1
sudo usermod -a -G penguins tux1
sudo useradd --create-home tux2
sudo usermod -a -G penguins tux2
sudo passwd tux1 < password1.sh
sudo passwd tux2 < password2.sh

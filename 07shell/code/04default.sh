#!/bin/sh

a=${1-10} # ${y-v} $y is null return $v
b=${2-30}

echo $(( $a + $b ))
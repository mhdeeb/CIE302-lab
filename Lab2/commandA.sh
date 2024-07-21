#!/bin/sh

mkdir lab                                                         # 1
cp numbers words lab/                                             # 2
cd lab/
paste words numbers > MergeContent                                # 3
head -n 3 MergeContent                                            # 4
sort MergeContent > SortedMergedContent                           # 5
tmpfile=$(mktemp)                                                 # 6
tr < SortedMergedContent [:lower:] [:upper:] > ${tmpfile}         # 6
cat ${tmpfile} > SortedMergedContent                              # 6
grep -P "^w.+[[:digit:]]$" MergeContent                           # 7
tr < SortedMergedContent I O > NewMergeContent                    # 8
paste MergeContent NewMergeContent                                # 9

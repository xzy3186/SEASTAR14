#!/bin/bash

for RUN in {201..355}
do
   if [ $RUN -lt 10 ]; then
      touch conv00$RUN.C
      echo "{" >conv00$RUN.C
      echo -e "\t gROOT->ProcessLine(\".L macros/MakeAllTreePieter.C+\");" >>conv00$RUN.C
      echo -e "\t MakeAllTreePieter(\"ridf/66Cr/psp14000${RUN}.ridf\",\"rootfiles/66Cr/psp14000${RUN}_PID.root\");" >>conv00$RUN.C
      echo "}" >>conv00$RUN.C
   elif [ $RUN -ge 10 ] && [ $RUN -lt 100 ]; then
      touch conv0$RUN.C
      echo "{" >conv0$RUN.C
      echo -e "\t gROOT->ProcessLine(\".L macros/MakeAllTreePieter.C+\");" >>conv0$RUN.C
      echo -e "\t MakeAllTreePieter(\"ridf/66Cr/psp1400${RUN}.ridf\",\"rootfiles/66Cr/psp1400${RUN}_PID.root\");" >>conv0$RUN.C
      echo "}" >>conv0$RUN.C
   elif [ $RUN -ge 100 ] && [ $RUN -lt 200 ]; then
      touch conv$RUN.C
      echo "{" >conv$RUN.C
      echo -e "\t gROOT->ProcessLine(\".L macros/MakeAllTreePieter.C+\");" >>conv$RUN.C
      echo -e "\t MakeAllTreePieter(\"ridf/72Fe/psp140${RUN}.ridf\",\"rootfiles/72Fe/psp140${RUN}_PID.root\");" >>conv$RUN.C
      echo "}" >>conv$RUN.C
   elif [ $RUN -ge 200 ] && [ $RUN -lt 1000 ]; then
      touch conv$RUN.C
      echo "{" >conv$RUN.C
      echo -e "\t gROOT->ProcessLine(\".L macros/MakeAllTreePieter.C+\");" >>conv$RUN.C
      echo -e "\t MakeAllTreePieter(\"ridf/78Ni/psp140${RUN}.ridf\",\"rootfiles/78Ni/psp140${RUN}_PID.root\");" >>conv$RUN.C
      echo "}" >>conv$RUN.C
   fi
done

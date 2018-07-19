#!/bin/sh

function mapp() {
#  This is from, http://prll.sourceforge.net/shell_parallel.html
  if [[ -z $MAPP_NR_CPUS ]] ; then
#	local MAPP_NR_CPUS=$(grep "processor	:" < /proc/cpuinfo | wc -l)
#   max core for calculation; modified by Toshiyuki Sumikama
      local MAPP_NR_CPUS=1
  fi
  local mapp_pid=$(exec bash -c 'echo $PPID')
  local mapp_funname=$1
  local -a mapp_params
  mapp_params=("$@")
#   mapp_nr_args; modified by Toshiyuki Sumikama
#  local mapp_nr_args=${#mapp_params[@]}
  local mapp_nr_args=`expr ${#mapp_params[@]} - 1`
  local mapp_current=0
  function mapp_trap() {
#	echo "MAPP PROGRESS: $((mapp_current*100/mapp_nr_args))%" 1>&2
	if [[ $mapp_current -lt $mapp_nr_args ]] ; then
	    let mapp_current+=1
	    (
		$mapp_funname "${mapp_params[$mapp_current]}"
		kill -USR1 $mapp_pid
	    ) &
	fi
  }

  trap mapp_trap SIGUSR1
  while [[ $mapp_current -lt $mapp_nr_args ]]; do
	wait
	if [[ $mapp_current -lt $mapp_nr_args && $? -lt 127 ]] ; then
	    sleep 1
	    local mapp_tmp_count=$mapp_current
	    wait
	    if [[ $mapp_tmp_count -eq $mapp_current ]] ; then
		echo "   MAPP_FORCE" 1>&2
		for i in $(seq 1 ${MAPP_NR_CPUS}) ; do
		    mapp_trap
		done
	    fi
	fi
  done
  for i in $(seq 1 ${MAPP_NR_CPUS}) ; do
	wait
  done
  trap - SIGUSR1
  unset -f mapp_trap
}

myfun()
{
   (
   cd /home/xzy3186/analysis/psp14/users/xuzy
   #./WASABI2Tr $1 >> /dev/null
   #./DSSDGeMerge $1 $1 >> /dev/null
   #./separate $1 $1 >> /dev/null
   #./BuildDecay $1 >> /dev/null
   #./BuildDecay_Select2 $1 >> /dev/null
   #./MakeGGMatrixDecay $1 >> /dev/null
   #./MakeGGMatrixDecayOneIsotope2 $1 >> /dev/null
   #./MakeGGMatrixOneIsotope $1 >> /dev/null
   #./MakeGGMatrix $1 >> /dev/null
   #if [ $1 -lt 10 ]; then
   #   root -l -b -q /home/analysis/analysis/anaroot/users/xuzy/script/conv00${1}.C >>/dev/null
   #elif [ $1 -ge 10 ] && [ $1 -lt 100 ]; then
   #   root -l -b -q /home/analysis/analysis/anaroot/users/xuzy/script/conv0${1}.C >>/dev/null
   #elif [ $1 -ge 100 ]; then
   #   root -l -b -q /home/analysis/analysis/anaroot/users/xuzy/script/conv${1}.C >log.txt 2>&1
   #fi
   ./macros/MakeMINOSOfflineCut ridf/78Ni/psp140${1}.ridf rootfiles/78Ni/psp140${1}_77Ni_new.root >>/dev/null
   #./macros/MakeMINOSVDrift ridf/78Ni/psp140${1}.ridf rootfiles/78Ni/psp140${1}_VDrift.root >> /dev/null
   echo $1' was finished.'
   )
}

#for eachrun in {206..216} #for dali calib on 2014-05-07
#for eachrun in {217..244} #for dali calib on 2014-05-08
#for eachrun in {245..270} #for dali calib on 2014-05-09
#for eachrun in {271..300} #for dali calib on 2014-05-10
#for eachrun in {301..324} #for dali calib on 2014-05-11
#for eachrun in {325..347} #for dali calib on 2014-05-12
for eachrun in {348..355} #for dali calib on 2014-05-13
#for eachrun in {206..206}
do
   list=$list' '$eachrun
done

echo $list
#anarootlogin
source $HOME/analysis/psp14/anaroot/scripts/anarootlogin.sh xuzy >> /dev/null

(mapp myfun $list)


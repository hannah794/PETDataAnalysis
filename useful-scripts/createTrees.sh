runNo=$1
echo ${1}
command="./makeTree -i /home/lhcbuser/PET-setup/RICHECKIT/data/feb-45F6379E-000${1}.mdf -o out-feb-45F6379E-000${1}.root -m AB"
echo ${command}
${command}
command="./makeTree -i /home/lhcbuser/PET-setup/RICHECKIT/data/feb-4944044F-000${1}.mdf -o out-feb-4944044F-000${1}.root -m AB"
echo ${command}
${command}

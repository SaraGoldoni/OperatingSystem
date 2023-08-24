#!/bin/bash
echo "1.Aggiungi"
echo "2.Elimina"
echo "3.Visualizza elenco"
echo "4.Ricerca"
echo "5.Iscritti per anno"
echo "6.Esporta passwd"
echo "7.Esci"
read opzione

if [[ -z $1 ]]
then
 set $1 "./studenti.txt"
fi
touch $1
while [ $opzione -gt 7 ]
do
echo "Errore, hai inserito un numero non valido, riprova"
read opzione
done

while [ "$opzione" != 7 ]

do

case  $opzione in
1)
echo "Inserire nome:"
read nome
nome=${nome^}
while [[ -z $nome ]] || [[ $nome =~ [0-9] ]]
do
echo "Nessun nome inserito, riprova"
read nome
nome=${nome^}
done

echo "Inserire cognome:"
read cognome
cognome=${cognome^}
while [[ -z $cognome ]] || [[ $cognome =~ [0-9] ]]
do
echo "Nessun cognome inserito, riprova"
read cognome
cognome=${cognome^}
done


echo "Inserire matricola:"
read matricola
while [[ ! ${#matricola} -eq 6 ]] || [[ $matricola =~ [a-Z] ]]
do
echo "La matricola inserita non cotiene 6 numeri, riprova"
read matricola
done
echo "Inserire anno:"
read anno
while [[ $anno -le 0 ]]|| [[ $anno -ge 4 ]]
do 
echo "anno inserito non valido, inserire l'anno corretto"
read anno
done

#per controllare che la matricola non sia già inserita posso fare un grep con opzione -c che ritorna 0 o 1 in base al riultato della ricerca su file. Se trova una matrciola uguale ( ritorna 1) allora la matricola è già inserita e non va fatto l'inserimento, altrimenti se ritorna 0 va fatto l'inserimento.

if [ ! $(grep -c "$matricola" "$1") = 0 ]
then
echo "Matricola già presente nel database"
fi

if [ $(grep -c "$matricola" "$1") = 0 ]
then
printf "%s;%s;%s;%s;\n" "$nome" "$cognome" "$matricola" "$anno" >> "$1"
fi
echo "" ;;

2) echo "inserire matricola da eliminare"
read matricola
if [ $(grep -c "$matricola" "$1") = 0 ]
then
echo "la matricola inserita non esiste"
else
 sed -i /$matricola/d "$1" #il comando -i lo fa lavorare inplace nel file, se lo tolgo manda i comandi in output sulla shell
fi
echo "";;
3)
(echo "NOME;COGNOME;MATRICOLA;ANNO" && cat "$1")|sort  -k3 -n -t ";"| column -t -s ";"
echo "";;

4)echo "Inserire cognome da cercare"
read cognomericerca
cognomericerca=${cognomericerca^}
if [ $(grep -c "$cognomericerca" "$1") = 0 ]
then
echo "Errore, il cognome inserito non è presente nell'anagrafica"
else
i=1


while [ $i -le $(grep -cw $cognomericerca "$1") ]
do
echo "Nome:" $( sort "$1"| grep -w "$cognomericerca"  | head -n $i | tail -n 1| cut -d ";" -f1)
echo "Cognome:" $(sort "$1" | grep -w "$cognomericerca" | head -n $i | tail -n 1 | cut -d ";" -f2)
echo "Matricola:" $(sort "$1" |grep -w "$cognomericerca" | cut -d ";" -f3 | head -n $i | tail -n 1)
echo "Anno:" $(sort "$1" |grep -w "$cognomericerca" | cut -d ";" -f4 | head -n $i | tail -n 1)
echo ""
let "i= i+1"
done
fi
;;


5)
echo ""
echo "Primo anno:" $(cut -d ";" -f4 "$1"| grep -o "1" | wc -l)
echo "Secondo anno:" $( cut -d ";" -f4 "$1"| grep -o "2" | wc -l)
echo "Terzo anno:" $(cut -d ";" -f 4 "$1" | grep -o "3" | wc -l)
;;
6) 
echo "inserire il percorso file e il nome del file"
read file

echo "Inserire UID" 
read uid
echo "Inserire gruppo di appartenenza"
read gid
echo "inserire la shell"
read shell

while [ ! $(grep -w "$shell" /etc/shells) ]
do
echo " la shell non esiste, ritenta"
read shell
done
j=1
while [ $j -le $( wc -l < "$1" ) ]
do
printf "%s:x:%s:%s:%s,%s:/home/%s:%s;\n" "$( cut -d ";" -f3 "$1"| head -n $j | tail -n 1)" "$uid" "$gid" "$( cut -d ";" -f2 "$1" | head -n $j | tail -n 1)" "$( cut -d ";" -f1 "$1" | head -n $j | tail -n 1)" "$( cut -d ";" -f3 "$1"| head -n $j | tail -n 1)" "$shell"
let "uid=uid+1"
let "j=j+1"
done > $file

;;

7) exit;;
esac;
echo ""
echo "1.Aggiungi"
echo "2.Elimina"
echo "3.Visualizza elenco"
echo "4.Ricerca"
echo "5.Iscritti per anno"
echo "6.Esporta passwd"
echo "7.Esci"
read opzione

done

# VR473361
# Sara Goldoni
# 11/12/21
# gestioneAnagrafica.sh

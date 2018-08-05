
while true
do
    VAR=`ps -af | grep -c "java"`
    if [ $VAR = "1" ]; then
        java -jar locam.jar "/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out" "/Users/oda/LegendsOfCodeAndMagic/NWTraining/a.out" /Users/oda/hsai/trained_model/tmp /Users/oda/LegendsOfCodeAndMagic/NWTraining/log
    fi
done
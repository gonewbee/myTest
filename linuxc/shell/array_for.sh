dirs=(drawable drawable-hdpi drawable-ldpi)
for dir in ${dirs[@]}
do
    echo $dir
    mkdir /tmp/$dir
done

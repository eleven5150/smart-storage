<?php

    require "config.php";

    $connection = mysqli_connect(
        $config['db']['server'],
        $config['db']['username'],
        $config['db']['password'],
        $config['db']['name']
    ); //функция возвращает спец. тип данных - ресурс, содержит ссылку на внешний ресурс 

    if($connection == false)
    {
        echo 'Не удалось подключиться к базе данных<br>';
        echo mysqli_connect_error(); //возвращает тестовое описание последней ошибки MySQL
        exit(); //прерывание работы скрипта
    }
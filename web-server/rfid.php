<?php

include('./includes/db_connect.php');
require "./vendor/autoload.php";
require "./protos/export/LedStripData_t.php";
require "./protos/export/Respond_t.php";
require "./protos/export/RgbData_t.php";
require "./protos/export/GPBMetadata/LedStripData.php";

use Google\Protobuf\Internal\InputStream;
use Google\Protobuf\Internal\Message;

@$newSocket = fsockopen("tcp://192.168.43.252", 333, $errno, $errstr, 10); //open socket
if (!$newSocket) //error condition
{
    $result = [
        'answer' => "Error with connection to LED",
    ];
    echo json_encode($result);
} else {

    $send = "rfid";
    fwrite($newSocket, $send); // send data to STM32
    $data = fread($newSocket, 4096); // Respond from STM32
    if ($data)
    {
//        $sql = "INSERT INTO `resistors`(`cell_number`, `part_number`, `package`, `resistance`, `tolerance`, `power`, `max_voltage`, `amount`, `url`)
//            VALUES (14,'0402','SMD',10,5,0.625,100,30,'https://www.chipdip.ru/product/0.062w-0402-1-om-1')";
//        mysqli_query($connection, $sql);
        $result =
            [
                'answer' => "Добавлено",
            ];
    }
    else
    {
        $result =
            [
                'answer' => "Ошибка",
            ];
    }
    fclose($newSocket);
    echo json_encode($result); // Переводим массив в JSON
}


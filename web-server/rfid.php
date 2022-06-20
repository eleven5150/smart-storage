<?php

require "./includes/config.php";
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

    $send = "rf";
    fwrite($newSocket, $send); // send data to STM32
    $data = fread($newSocket, 1); // Respond from STM32
    if ($data)
    {
        $sql = "INSERT INTO `resistors`(`cell_number`, `part_number`, `package`, `resistance`, `tolerance`, `power`, `max_voltage`, `amount`, `url`
, `boardNumber`, `stripNumber`, `x_coordinate`, `y_coordinate`, `R`, `G`, `B`)
            VALUES (14,'0402','SMD',10,5,0.625,100,30,'https://www.chipdip.ru/product/0.062w-0402-1-om-1',NULL, NULL, NULL, NULL,NULL,NULL,NULL)";
        $connection = mysqli_connect(
            $config['db']['server'],
            $config['db']['username'],
            $config['db']['password'],
            $config['db']['name']
        );
        $status = mysqli_query($connection, $sql);
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


<?php
    require "./vendor/autoload.php";
    require "./protos/export/LedStripData_t.php";
    require "./protos/export/Respond_t.php";
    require "./protos/export/RgbData_t.php";
    require "./protos/export/GPBMetadata/LedStripData.php";

    use Google\Protobuf\Internal\InputStream;
    use Google\Protobuf\Internal\Message;

    @$newSocket = fsockopen("tcp://192.168.43.252", 333, $errno, $errstr, 1); //open socket
    if (!$newSocket) //error condition
    {
        $result = [
        'answer' => "Error with connection to LED",
        ];
        echo json_encode($result);
    }
    else
    {
//             $transm = new LedStripData_t();
//             $transmColour = new RgbData_t();
//             $transm->setBoardNumber($_POST["boardNumber"]);
//             $transm->setStripNumber($_POST["stripNumber"]);
//             $transm->setStartLedNumber($_POST["x_coordinate"]);
//             $transm->setEndLedNumber($_POST["y_coordinate"]);
//             $transmColour->setRed($_POST["R"]);
//             $transmColour->setGreen($_POST["G"]);
//             $transmColour->setBlue($_POST["B"]);
//             $transm->setColour($transmColour);
//             $serString = $transm->SerializeToString();
                $xy = $_POST["x_coordinate"] * 10 + $_POST["y_coordinate"];
             fwrite($newSocket, $xy); // send data to STM32
//             $received = new Respond_t();
//             $received->mergeFromString(fread($newSocket, 4096)); // Respond from STM32

        $result =
        [
            'answer' => "OK",
        ];
        fclose($newSocket);
        echo json_encode($result); // Переводим массив в JSON
    }
?>

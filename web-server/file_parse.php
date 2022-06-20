<?php
    include('includes/db_connect.php');
    $parse_respond =array(); //переменная с данными для возврата в index.php

    if(isset($_POST['cat_id'])) {
        $cat_name = $_POST['cat_id'];
    }

    if (!is_uploaded_file($_FILES['uploadfile']['tmp_name'])) {
        $parse_respond['errors'] = "Файл не загружен на сервер";
    }

    if(($_FILES['uploadfile']['type'] != 'application/vnd.ms-excel') && ($_FILES['uploadfile']['type'] != 'text/csv')) {
       $parse_respond['errors'] = 'Неправильный тип файла. Загрузите файл с расширением .csv';
    }

    if(isset($_POST['new_cat'])) {
        $new_cat = $_POST['new_cat'];
    }

    // Запрос на получение имен столбцов выбранной таблицы
    $sql = "SHOW COLUMNS FROM `$cat_name`";
    $result_q = mysqli_query($connection, $sql);
    if($result_q == false) {
        $parse_respond['errors'] = 'Произошла ошибка при выполнении запроса (не удалось получить имена столбцов таблицы из БД)';
        mysqli_error($connection);
    }

    $counter = 0; //переменная для подсчёта кол-ва столбцов таблицы
    $result = array(); //массив, в котором хранятся названия стобцов таблицы
    while($record = mysqli_fetch_assoc($result_q))
    {
        if($record['Field'] == 'id') {
            continue;
        }
        $result[$counter] = $record['Field']; 
        $counter++;
    }

    //Получение значений part_number из таблицы БД
    $sql_p = "SELECT `part_number` FROM `$cat_name`";
    $result_p =  mysqli_query($connection, $sql_p);
    if($result_p == false) {
        $parse_respond['errors'] = 'Произошла ошибка при выполнении запроса (не удалось получить значения столбца part_number из таблицы БД)';
        mysqli_error($connection);
    }

    $counter_p = 0; 
    $result_pn = array(); //массив, в котором хранятся названия part_number таблицы БД
    while($record_p = mysqli_fetch_assoc($result_p))
    {
        $result_pn[$counter_p] = $record_p; 
        $counter_p++;
    }

    // Каталог, в который мы будем принимать файл
    $uploaddir = './files/';
    $uploadfile = $uploaddir.basename($_FILES['uploadfile']['name']);

    // Копируем файл из каталога для временного хранения файлов
    if (copy($_FILES['uploadfile']['tmp_name'], $uploadfile))
    {
        $parse_respond['name'] = $_FILES['uploadfile']['name']; //возвращение названия файла при успешной загрузке
    }
    else 
    { 
        $parse_respond['errors'] = "<h3>Ошибка! Не удалось загрузить файл на сервер!</h3>";
    }

    // // Выводим информацию о загруженном файле:
    // //echo "<h3>Информация о загруженном на сервер файле: </h3>";
    // //echo "<p><b>Оригинальное имя загруженного файла: ".$_FILES['uploadfile']['name']."</b></p>";
    // //echo "<p><b>Mime-тип загруженного файла: ".$_FILES['uploadfile']['type']."</b></p>";
    // //echo "<p><b>Размер загруженного файла в байтах: ".$_FILES['uploadfile']['size']."</b></p>";
    // //echo "<p><b>Временное имя файла: ".$_FILES['uploadfile']['tmp_name']."</b></p>";

    $row = 0;
    $num1 = 0;
    if (($handle = fopen("./files/" . $_FILES['uploadfile']['name'], "r")) != FALSE) {
        while (($data = fgetcsv($handle, 1000, ";")) != FALSE) {
            // Проверка на совпадение ячеек первой строки файла с именами столбцов таблицы БД
            if($row == 0) {
                $num1 = count($data);
                for ($c = 0; $c < $num1; $c++) {
                    //echo $data[$c] . ' <b>' . $result[$c] . '</b><br>';
                    if($result[$c] != $data[$c]) {
                        $parse_respond['errors'] = '<p>Неправильный формат столбцов таблицы</p>';
                    }
                }
                $output = implode("`, `", $result);

                $row++;
                continue;
            }

            $data_r = array();
            for ($c = 0; $c < $num1; $c++) {           
                if($data[$c] == '')
                {
                    $data_r[$c] = 'NULL'; 
                }
                else
                {
                    $data_r[$c] = $data[$c];
                }
            }

            $output_d = implode(", ", $data_r);
            
            $sql = "INSERT INTO `$cat_name` (`$output`) VALUES ($output_d)";

            $same_item_flag = false;
            for($i = 0; $i < $counter_p; $i++) {
                //echo '<p><b>' . $data[1] . ' and ' . $result_pn[$i]['part_number'] . '</b></p>';
                if($data[1] == "'" . $result_pn[$i]['part_number'] . "'") {
                    $same_item_flag = true;
                    break;
                }
            }

            if($same_item_flag == false) {
                $result_e = mysqli_query($connection, $sql);
                if ($result_e == false) {
                    $parse_respond['errors'] = 'Парсинг строки №' . $row . ' НЕ удался<br>';
                   // echo 'Ошибка при выполнении запроса: ';
                   // echo '<b>' . mysqli_error($connection) . '</b><br>';
                    //echo '<p>SQL запрос: ' . $sql . '</p>';
                } else {
                    $parse_respond['errors'] = 'Парсинг строки №' . $row . ' удался<br>';
                    //echo '<p>SQL запрос:<br>' . $sql . '</p>';
                }
            } else {
                $parse_respond['errors'] = 'Компонент в строке №' . $row . ' уже имеется в таблице базы данных<br>';
                //echo '<p>SQL запрос:<br>' . $sql . '</p>';  
            }
            
            $row++;
        }
        fclose($handle);
    }

    //возврашение значения в index.php
    $res = '<script type="text/javascript">';
    $res .= "var parse_respond = new Object;";
    foreach($parse_respond as $key => $value){
        $res .= 'parse_respond.'.$key.' = "'.$value.'";';
    }
    $res .= 'window.parent.handleResponse(parse_respond);';
    $res .= "</script>";
         
    echo $res;  
?>
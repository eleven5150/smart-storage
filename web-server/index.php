<?php
    require "includes/db_connect.php";
    require "includes/config.php";
?>

<!doctype html>
<html>
    <head>
        <script src="jquery-3.6.0.min.js"></script>
        <script type="text/javascript"> //скрипт обработки динамического импорта файлов
            function hideBtn(){
                $('#upload').hide();
                $('#respondparse').html("Идет загрузка файла");
            }

            function handleResponse(mes) {
                //document.write("check");
                $('#upload').show();

                if (mes.errors != null) {
                    $('#respondparse').html("Возникли ошибки во время загрузки файла: " + mes.errors);
                }   
                else {
                    $('#respondparse').html("Файл " + mes.name + " загружен");   
                }   
            }
        </script>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <!-- Bootstrap CSS -->
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-F3w7mX95PdgyTmZZMECAngseQB83DfGTowi0iMjiWaeVhAn4FJkqJByhZMI3AhiU" crossorigin="anonymous">
        
        <link rel="canonical" href="https://getbootstrap.com/docs/5.1/examples/starter-template/">

        <style>
        .bd-placeholder-img {
            font-size: 1.125rem;
            text-anchor: middle;
            -webkit-user-select: none;
            -moz-user-select: none;
            user-select: none;
        }

        @media (min-width: 768px) {
            .bd-placeholder-img-lg {
                font-size: 3.5rem;
            }
        }
        </style>

        <!-- Custom styles for this template -->
        <link href="starter-template.css" rel="stylesheet">

        <title>Умный склад</title>
    </head>

    <body>
        <!-- Bootstrap в связке с Popper -->
        <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-/bQdsTh/da6pkI1MST/rWKFNjaCP5gBSY4sEBT38Q/9RBh9AH40zEOg7Hlq2THRZ" crossorigin="anonymous"></script>
        
        <div class="col-lg-8 mx-auto p-3 py-md-5">
            <main>
                <h1>Умный склад</h1>
                <p class="fs-5 col-md-8">Cистема автоматического поиска комплектующих на рабочих местах инженеров-электронщиков</p>
                <hr class="col-3 col-md-2 mb-5">
                <div class="row g-5">
                <div class="col-md-6">
                    <h2>Категории компонентов</h2>
                    <p>Выберите категорию, чтобы перейти к таблице электронных компонентов.</p>
                    <?php
                        $sql = 'SELECT * FROM `components_categories`';
                        $result = mysqli_query($connection, $sql); //спец. тип данных, ссылка на результаты запросы (для SELECT)

                        if($result == false) {
                        echo 'Произошла ошибка';
                        mysqli_error($connection);
                        }

                    ?>

                    <ul class="icon-list">
                        <?php
                            while ($cat = mysqli_fetch_assoc($result)) //функция возвращет данные в виде ассоциативного массива
                            {
                        ?>
                                <li><a href="/category.php?id=<?php echo $cat['id']; ?>" class="btn btn-outline-primary btn-sm"><?php echo $config[$cat['category_name']][$cat['category_name']]; ?></a></li>
                        <?php
                            }
                        ?>
                    </ul>

                </div>

                <?php
                    $sql = 'SELECT * FROM `components_categories`';
                    $result = mysqli_query($connection, $sql); //спец. тип данных, ссылка на результаты запросы (для SELECT)
                ?>

                <div class="col-md-6">
                    <h2>Импорт компонентов в базу данных</h2>
                    <p>Загрузите файл в расширении .csv для добавления компонентов в базу данных. Файл .csv должен соответствовать определённому формат. Формат файлов задаётся администратором.</p>
                    <div>
                        <div class="bd-example">
                            <form action="file_parse.php" method="post" enctype="multipart/form-data" id="parse" name="parse" target="hiddenframe" onsubmit="hideBtn();">  <!-- old action /file_parse.php -->
                                 <div class="mb-3">
                                    <label class="form-label">Выберите категорию</label>
                                    <select class="form-select" name="cat_id" id="cat_id">
                                    <?php
                                        while($cat = mysqli_fetch_assoc($result)) { 
                                    ?>
                                        <option value="<?php echo $cat['category_name'] ?>"><?php echo $config[$cat['category_name']][$cat['category_name']]; ?></option>
                                    <?php
                                        }
                                    ?>
                                    </select>
                                </div>
                                <div class="mb-3">
                                    <input type="file" class="form-control" name="uploadfile" id="uploadfile">
                                </div>
                                <button type="submit" class="btn btn-primary" name="upload" id="upload">Загрузить</button> 
                            </form>
                        </div>
                    </div>

                    <div id="respondparse"></div>

                    <iframe id="hiddenframe" name="hiddenframe" style="width:0px; height:0px; border:0px"></iframe>
                </div>

                <!--
                Форма добавления новой категории компонентов
                <div>
                    <form action="/new_category.php" method="post" enctype="multipart/form-data">
                        <p><strong>Название новой категории компонентов (на русском)</strong><br>
                        <input type="text" size="40" name="new_cat_ru">
                        </p>
                        <p><strong>Название новой категории компонентов (на английском, без пробелов)</strong><br>
                        <input type="text" size="40" name="new_cat">
                        </p>
                        <p>
                        <input type="file" name="uploadfile">
                        <input type="submit" value="Загрузить">
                        </p>
                    </form>
                </div>
                -->

                <?php
                    mysqli_close($connection);
                ?>
                </div>
            </main>

        <footer class="pt-5 my-5 text-muted border-top">
            Кутаев Кирилл и Рыжов Фёдор для RaccHack &middot; 2021
        </footer>
    </body>
</html>
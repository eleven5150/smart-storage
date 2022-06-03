<?php

    $config = array(
        'title' => 'Умный склад',

        'db' => array(
            'server' => 'localhost',
            'username' => 'root',
            'password' => 'root',
            'name' => 'components_db'
        ),

        'active_buttons' => 'Активные кнопки',

        'electrolytic_capacitors' => array(
            'electrolytic_capacitors' => 'Электролетические конденсаторы',
            'id' => 'id',
            'cell_number' => 'Номер ячейки',
            'part_number' => 'Part Number',
            'package' => 'Корпус',
            'voltage' => 'Макс. напряжение, В',
            'capacitance' => 'Ёмкость, мкФ',
            'tolerance' => 'Точность, %',
            'amount' => 'Колличество, шт',
            'url' => 'Ссылка'
        ),

        'resistors' => array(
            'resistors' => 'Резисторы',
            'id' => 'id',
            'cell_number' => 'Номер ячейки',
            'part_number' => 'Part Number',
            'package' => 'Корпус',
            'resistance' => 'Сопротивление, Ом',
            'tolerance' => 'Точность, %',
            'power' => 'Мощность, Вт',
            'max_voltage' => 'Макс. напряжение, В',
            'amount' => 'Колличество, шт',
            'url' => 'Ссылка',
            'boardNumber' => 'Номер борды',
            'stripNumber' => 'Номер ленты',
            'startLedNumber' => 'Первый светодиод',
            'endLedNumber' => 'Последний светодиод',
            'R' => 'Красный',
            'G' => 'Зеленый',
            'B' => 'Синий',
        ),
        
        'bipolar_transistors' => array(
            'bipolar_transistors' => 'Биполярные транзисторы',
            'id' => 'id',
            'cell_number' => 'Номер ячейки',
            'part_number' => 'Part Number',
            'package' => 'Корпус',
            'structure' => 'Структура',
            'Vcbo_max' => 'Vcbo max, В',
            'Vceo_max' => 'Vceo max, В',
            'Ic_max' => 'Ic max, А',
            'power' => 'Мощность, Вт',
            'amount' => 'Колличество, шт',
            'url' => 'Ссылка',
        )
    );

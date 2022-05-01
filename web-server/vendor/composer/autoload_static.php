<?php

// autoload_static.php @generated by Composer

namespace Composer\Autoload;

class ComposerStaticInit929fecfcb05032aaf301147a0d693e26
{
    public static $prefixLengthsPsr4 = array (
        'G' => 
        array (
            'Google\\Protobuf\\' => 16,
            'GPBMetadata\\Google\\Protobuf\\' => 28,
        ),
    );

    public static $prefixDirsPsr4 = array (
        'Google\\Protobuf\\' => 
        array (
            0 => __DIR__ . '/..' . '/google/protobuf/src/Google/Protobuf',
        ),
        'GPBMetadata\\Google\\Protobuf\\' => 
        array (
            0 => __DIR__ . '/..' . '/google/protobuf/src/GPBMetadata/Google/Protobuf',
        ),
    );

    public static $classMap = array (
        'Composer\\InstalledVersions' => __DIR__ . '/..' . '/composer/InstalledVersions.php',
    );

    public static function getInitializer(ClassLoader $loader)
    {
        return \Closure::bind(function () use ($loader) {
            $loader->prefixLengthsPsr4 = ComposerStaticInit929fecfcb05032aaf301147a0d693e26::$prefixLengthsPsr4;
            $loader->prefixDirsPsr4 = ComposerStaticInit929fecfcb05032aaf301147a0d693e26::$prefixDirsPsr4;
            $loader->classMap = ComposerStaticInit929fecfcb05032aaf301147a0d693e26::$classMap;

        }, null, ClassLoader::class);
    }
}

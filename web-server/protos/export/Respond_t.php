<?php
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: LedStripData.proto

use Google\Protobuf\Internal\GPBType;
use Google\Protobuf\Internal\RepeatedField;
use Google\Protobuf\Internal\GPBUtil;

/**
 * Generated from protobuf message <code>Respond_t</code>
 */
class Respond_t extends \Google\Protobuf\Internal\Message
{
    /**
     * Generated from protobuf field <code>uint32 code = 1;</code>
     */
    protected $code = 0;

    /**
     * Constructor.
     *
     * @param array $data {
     *     Optional. Data for populating the Message object.
     *
     *     @type int $code
     * }
     */
    public function __construct($data = NULL) {
        \GPBMetadata\LedStripData::initOnce();
        parent::__construct($data);
    }

    /**
     * Generated from protobuf field <code>uint32 code = 1;</code>
     * @return int
     */
    public function getCode()
    {
        return $this->code;
    }

    /**
     * Generated from protobuf field <code>uint32 code = 1;</code>
     * @param int $var
     * @return $this
     */
    public function setCode($var)
    {
        GPBUtil::checkUint32($var);
        $this->code = $var;

        return $this;
    }

}

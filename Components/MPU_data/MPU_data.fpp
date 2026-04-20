module Components {

    @ 3-tuple type used for telemetry
    array Vector = [3] F32
    
    port MPU_data (
        acceleration: Vector
    )
}

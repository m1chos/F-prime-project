module Components {

    array  HumVector = [1] F32
    array  TempVector = [1] F32
    
    port HUM_data (
        humidity: HumVector
        temperature: TempVector 
    )
}

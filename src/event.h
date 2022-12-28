#include <variant>
#include <stdint.h>
#include <vector>

enum class FlashEraseType : uint8_t {
    SECTOR_4KB = 0,
    BLOCK_32KB = 1,
    BLOCK_64KB = 2,
    CHIP = 3,
};

enum class FlashStatusType {
    IDLE = 0,
    BUSY = 1,
};

struct Spike {
    uint8_t layer;
    uint16_t feature;
    uint8_t y;
    uint8_t x;
    uint32_t timestamp;

    bool operator==(const Spike& other) const
    {
        return this->layer == other.layer &&
               this->feature == other.feature &&
               this->y == other.y &&
               this->x == other.x &&
               this->timestamp == other.timestamp;
    }
};

struct KillSensorPixel {
    uint8_t y;
    uint8_t x;

    bool operator==(const KillSensorPixel& other) const
    {
        return this->y == other.y &&
               this->x == other.x;
    }
};

struct ResetSensorPixel {
    bool operator==(const ResetSensorPixel&) const
    {
        return true;
    }
};

struct WriteNeuronValue {
    uint8_t layer;
    uint32_t address;
    int16_t neuronState;

    bool operator==(const WriteNeuronValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address &&
               this->neuronState == other.neuronState;
    }
};

struct ReadNeuronValue {
    uint8_t layer;
    uint32_t address;

    bool operator==(const ReadNeuronValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address;
    }
};

struct WriteWeightValue {
    uint8_t layer;
    uint32_t address;
    int8_t weight;

    bool operator==(const WriteWeightValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address &&
               this->weight == other.weight;
    }
};

struct ReadWeightValue {
    uint8_t layer;
    uint32_t address;

    bool operator==(const ReadWeightValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address;
    }
};

struct WriteBiasValue {
    uint8_t layer;
    uint32_t address;
    int16_t bias;

    bool operator==(const WriteBiasValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address &&
               this->bias == other.bias;
    }
};

struct ReadBiasValue {
    uint8_t layer;
    uint32_t address;

    bool operator==(const ReadBiasValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address;
    }
};

struct WriteRegisterValue {
    // register addr only takes the first 16-bit of 24-bit
    uint16_t address;
    uint8_t data;

    bool operator==(const WriteRegisterValue& other) const
    {
        return this->address == other.address &&
               this->data == other.data;
    }
};

struct ReadRegisterValue {
    uint16_t address;

    bool operator==(const ReadRegisterValue& other) const
    {
        return this->address == other.address;
    }
};

struct DvsEvent {
    bool p = false;
    uint8_t y = 0;
    uint8_t x = 0;
    uint32_t timestamp = 0;

    bool operator==(const DvsEvent& other) const
    {
        return this->p == other.p &&
               this->y == other.y &&
               this->x == other.x &&
               this->timestamp == other.timestamp;               
    }
};

struct InputInterfaceEvent {
    uint16_t value = 0;
    bool msp = false;

    bool operator==(const InputInterfaceEvent& other) const
    {
        return this->value == other.value &&
               this->msp == other.msp;
    }
};

struct NeuronValue {
    uint8_t layer;
    uint32_t address;
    int16_t neuronState = 0;

    bool operator==(const NeuronValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address &&
               this->neuronState == other.neuronState;
    }
};

struct BiasValue {
    uint8_t layer;
    uint32_t address;
    int16_t bias = 0;

    bool operator==(const BiasValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address &&
               this->bias == other.bias;
    }
};

struct WeightValue {
    uint8_t layer;
    uint32_t address;
    int8_t weight = 0;

    bool operator==(const WeightValue& other) const
    {
        return this->layer == other.layer &&
               this->address == other.address &&
               this->weight == other.weight;
    }
};

struct RegisterValue {
    uint16_t address;
    uint8_t data = 0;

    bool operator==(const RegisterValue& other) const
    {
        return this->address == other.address &&
               this->data == other.data;
    }
};

struct ReadoutValue {
    uint32_t value = 0;

    bool operator==(const ReadoutValue& other) const
    {
        return this->value == other.value;
    }
};

struct WriteFilterValue {
    uint8_t y;
    uint8_t x;
    uint16_t data;

    bool operator==(const WriteFilterValue& other) const
    {
        return this->x == other.x &&
               this->y == other.y &&
               this->data == other.data;
    }
};

struct ReadFilterValue {
    uint8_t y;
    uint8_t x;

    bool operator==(const ReadFilterValue& other) const
    {
        return this->x == other.x &&
               this->y == other.y;
    }
};

struct WriteSramValue {
    uint32_t address;
    uint8_t data;

    bool operator==(const WriteSramValue& other) const
    {
        return this->address == other.address &&
               this->data == other.data;
    }
};

struct ReadSramValue {
    uint32_t address;

    bool operator==(const ReadSramValue& other) const
    {
        return this->address == other.address;
    }
};

struct SramValue {
    uint32_t address;
    uint8_t data;

    bool operator==(const SramValue& other) const
    {
        return this->address == other.address &&
               this->data == other.data;
    }
};

struct FilterDvsEvent {
    bool p = false;
    uint8_t y;
    uint8_t x;

    bool operator==(const FilterDvsEvent& other) const
    {
        return this->x == other.x &&
               this->y == other.y &&
               this->p == other.p;
    }
};

struct FilterValue {
    uint8_t y;
    uint8_t x;
    uint16_t counterData;

    bool operator==(const FilterValue& other) const
    {
        return this->x == other.x &&
               this->y == other.y &&
               this->counterData == other.counterData;
    }
};

struct FilterValueCurrent {
    uint16_t counterData;

    bool operator==(const FilterValueCurrent& other) const
    {
        return this->counterData == other.counterData;
    }
};

struct FilterValuePrevious {
    uint16_t counterData;

    bool operator==(const FilterValuePrevious& other) const
    {
        return this->counterData == other.counterData;
    }
};

struct AsiRequest {
    bool operator==(const AsiRequest& other) const
    {
        return true;
    }
};

struct ReadoutRequest {
    bool operator==(const ReadoutRequest& other) const
    {
        return true;
    }
};

struct ScanChainValue {
    std::vector<uint8_t> data;
    uint32_t numberOfBits;
    uint8_t blockId;

    bool operator==(const ScanChainValue& other) const
    {
        return this->data == other.data &&
               this->numberOfBits == other.numberOfBits &&
               this->blockId == other.blockId;
    }
};

struct IdCodeRequest {
    bool operator==(const IdCodeRequest& other) const
    {
        return true;
    }
};

struct IdCode {
    uint32_t id;

    bool operator==(const IdCode& other) const
    {
        return this->id == other.id;
    }
};

struct OutputModeSel {
    uint8_t modeSel;

    bool operator==(const OutputModeSel& other) const
    {
        return this->modeSel == other.modeSel;
    }
};

struct AverageSel {
    uint8_t averageSel;

    bool operator==(const AverageSel& other) const
    {
        return this->averageSel == other.averageSel;
    }
};

struct Frame {
    std::vector<uint16_t> data;
    bool operator==(const Frame& other) const
    {
        return data == other.data;
    }
};

struct FlashRead {
    uint32_t address;
    bool operator==(const FlashRead& other) const
    {
        return address == other.address;
    }
};

struct FlashValue {
    uint32_t address;
    uint8_t data;
    bool operator==(const FlashValue& other) const
    {
        return address == other.address &&
               data == other.data;
    }
};

struct FlashWrite {
    uint32_t address;
    uint32_t data;
    bool operator==(const FlashWrite& other) const
    {
        return address == other.address &&
               data == other.data;
    }
};

struct FlashErase {
    // 0: sector(4KB), 1: block(32KB), 2: block(64KB), 3: chip
    FlashEraseType type;
    uint32_t address;
    bool operator==(const FlashErase& other) const
    {
        return type == other.type &&
               address == other.address;
    }
};

struct FlashSpiRaw {
    std::vector<uint8_t> rawData;
    bool operator==(const FlashSpiRaw& other) const
    {
        return rawData == other.rawData;
    }
};

struct FlashWriteEnable {
    bool operator==(const FlashWriteEnable&) const
    {
        return false;
    }
};

struct FlashQueryStatus {
    bool operator==(const FlashQueryStatus&) const
    {
        return false;
    }
};

struct FlashStatus {
    FlashStatusType status;
    bool operator==(const FlashStatus& other) const
    {
        return status == other.status;
    }
};

struct ReadoutPinValue {
    int index; // represent the neuron index of max average
    uint32_t timestamp;

    bool operator==(const ReadoutPinValue& other) const
    {
        return this->index == other.index &&
               this->timestamp == other.timestamp;
    }
};

/// output event types for the device
using OutputEvent = std::variant<
    Spike,
    DvsEvent,
    InputInterfaceEvent,
    NeuronValue,
    BiasValue,
    WeightValue,
    RegisterValue,
    ReadoutValue,
    FilterValue,
    SramValue,
    FilterDvsEvent,
    FilterValueCurrent,
    FilterValuePrevious,
    ScanChainValue,
    IdCode,
    FlashValue,
    FlashSpiRaw,
    FlashStatus,
    ReadoutPinValue>;
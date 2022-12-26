#ifndef UUID_GEN_H_
#define UUID_GEN_H_

#include <uuid.h>
#include <memory>

std::unique_ptr<uuids::uuid_random_generator> createGenerator() {
    std::random_device rd;
    auto seed_data = std::array<int, std::mt19937::state_size> {};
    std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    std::mt19937 generator(seq);
    return std::make_unique<uuids::uuid_random_generator>(generator);
}

std::string getUuid()
{
    static auto gen = createGenerator();
    uuids::uuid const id = (*gen)();
    return uuids::to_string(id);
}


#endif
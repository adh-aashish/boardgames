#pragma once
#include <map>
#include <memory>
#include <iostream>


// Asset is like sf::Texture, ID is like AssetID::Crown
// This generic class can make object of fontholder,textureholder,soundholder
// but for music holder we need to make another class.
template <typename Asset, typename ID>
class AssetsManager
{
public:
    AssetsManager() {}
    ~AssetsManager() {}

    void load(ID id, const std::string &fileName);
    const Asset &getAsset(ID id) const;
    Asset &getAsset(ID id);

private:
    std::map<ID, std::unique_ptr<Asset>> mAssetsMap;
};

template <typename Asset, typename ID>
void AssetsManager<Asset, ID>::load(ID id, const std::string &fileName)
{
    std::unique_ptr<Asset> asset = std::make_unique<Asset>();
   
    if (!asset->loadFromFile(fileName))
    {
        throw std::runtime_error("ResourceHolder::load - Failed to load " + fileName);
    }
    mAssetsMap.insert(std::make_pair(id, std::move(asset)));
}

template <typename Asset, typename ID>
const Asset &AssetsManager<Asset, ID>::getAsset(ID id) const
{
    auto found = mAssetsMap.find(id);
    if (found != mAssetsMap.end())
    {
        // found
        return *(found->second);
    }
    else
    {
        throw std::runtime_error("Failed to get the requested asset" + std::to_string(id));
    }
}

template <typename Asset, typename ID>
Asset &AssetsManager<Asset, ID>::getAsset(ID id)
{
    auto found = mAssetsMap.find(id);
    if (found != mAssetsMap.end())
    {
        // found
        return *(found->second);
    }
    else
    {
        throw std::runtime_error("Failed to get the requested asset" + std::to_string(id));
    }
}
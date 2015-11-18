#pragma once

#include <memory>
#include <vector>
#include <opencv2/core/mat.hpp>
#include "Pose.h"

namespace yarrar {

class DataProvider
{
public:
    virtual ~DataProvider() {};
    virtual cv::Mat getData() = 0;
};

class Detector
{
public:
    virtual ~Detector() {};
    virtual Pose getPose(const cv::Mat& rawData) = 0;
};

class Renderer
{
public:
    virtual ~Renderer() {};
    virtual void draw(const Pose& cameraPose, const cv::Mat& rawData) = 0;
};

class Pipeline
{
public:

    template<
        typename T,
        typename... Args
    >
    void addDataProvider(Args&&... args)
    {
        m_dataProviders.emplace_back(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    void addDetector()
    {
        assert(m_dataProviders.size() == 1);
        auto image = m_dataProviders[0]->getData();
        m_detectors.emplace_back(new T(image.cols, image.rows));
    }

    template<typename T>
    void addRenderer()
    {
        assert(m_dataProviders.size() == 1);
        auto image = m_dataProviders[0]->getData();
        m_renderers.emplace_back(new T(image.cols, image.rows));
    }

    void run() const;

private:
    std::vector<std::unique_ptr<DataProvider>> m_dataProviders;
    std::vector<std::unique_ptr<Detector>> m_detectors;
    std::vector<std::unique_ptr<Renderer>> m_renderers;
};

}
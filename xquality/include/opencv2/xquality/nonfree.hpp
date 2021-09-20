#ifndef XQUALITY_NONFREE_HPP
#define XQUALITY_NONFREE_HPP

#include "opencv2/core.hpp"
#include "opencv2/quality.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/ml.hpp"

namespace cv
{
using quality::QualityBase;

namespace xquality
{

/**
@brief GMLOG (Blind/Referenceless Image Spatial Quality Evaluator) is a No Reference Image Quality Assessment (NR-IQA) algorithm.

BRISQUE computes a score based on extracting Natural Scene Statistics (https://en.wikipedia.org/wiki/Scene_statistics)
and calculating feature vectors. See Mittal et al. @cite Mittal2 for original paper and original implementation @cite Mittal2_software .

A trained model is provided in the /samples/ directory and is trained on the LIVE-R2 database @cite Sheikh as in the original implementation.
When evaluated against the TID2008 database @cite Ponomarenko , the SROCC is -0.8424 versus the SROCC of -0.8354 in the original implementation.
C++ code for the BRISQUE LIVE-R2 trainer and TID2008 evaluator are also provided in the /samples/ directory.
*/
class CV_EXPORTS_W QualityGMLOG : public QualityBase
{
public:

    /** @brief Computes GMLOG quality score for input image
    @param img Image for which to compute quality
    @returns cv::Scalar with the score in the first element.  The score ranges from 0 (best quality) to 100 (worst quality)
    */
    CV_WRAP virtual Scalar compute( InputArray img ) CV_OVERRIDE = 0;


    /**
    @brief static method for computing quality
    @param img image for which to compute quality.
    Note than for RGB and RGBA the score is computed for each colour channel, separtely, the last score is for the grayscale image computed from the colour channels.
    @param model_file_path cv::String which contains a path to the GMLOG model data, eg. /path/to/gmlog_model_live.yml
    @param range_file_path cv::String which contains a path to the GMLOG range data, eg. /path/to/gmlog_range_live.yml
    @returns cv::Scalar with the score in the first element.  The score ranges from 0 (best quality) to 100 (worst quality)
    */
    CV_WRAP virtual Scalar compute( InputArray img, const String& model_file_path, const String& range_file_path ) = 0;

    /**
    @brief static method for computing image features used by the GMLOG algorithm
    @param img image (BGR(A) or grayscale) for which to compute features
    @param features output row vector of features to cv::Mat or cv::UMat
    */
    CV_WRAP static void computeFeatures(InputArray img, OutputArray features);

    /**
    @brief method returning the name of the current algorithm
    */
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;

    /**
    @brief Create an object which calculates quality
    @param model_file_path cv::String which contains a path to the BRISQUE model data, eg. /path/to/gmlog_model_live.yml
    @param range_file_path cv::String which contains a path to the BRISQUE range data, eg. /path/to/gmlog_range_live.yml
    */
    static Ptr<QualityGMLOG> create( const String& model_file_path, const String& range_file_path );

    /**
    @brief Create an object which calculates quality
    @param model cv::Ptr<cv::ml::SVM> which contains a loaded GMLOG model
    @param range cv::Mat which contains GMLOG range data
    */
    static Ptr<QualityGMLOG> create( const Ptr<ml::SVM>& model, const Mat& range );


};  // QualityGMLOG

class CV_EXPORTS_W QualityBlockSVD : public QualityBase
{
public:

    virtual ~QualityBlockSVD() CV_OVERRIDE = default;


    /** @brief Implements Algorithm::empty()  */
    CV_WRAP virtual bool empty() const CV_OVERRIDE  = 0;

    /** @brief Implements Algorithm::clear()  */
    CV_WRAP virtual void clear() CV_OVERRIDE = 0;

    /**
    @brief Create an object which calculates quality
    @param ref input image to use as the reference for comparison
    */
    CV_WRAP static Ptr<QualityBlockSVD> create(InputArray ref = noArray(), const Size& blockSize = Size(8,8));

    /**
    @brief Create an object which calculates quality
    @param model_file_path cv::String which contains a path to the GMLOG model data, eg. /path/to/blocksvd_model_live.yml
    @param range_file_path cv::String which contains a path to the GMLOG range data, eg. /path/to/blocksvd_range_live.yml
    */
    CV_WRAP static Ptr<QualityBlockSVD> create( const String& model_file_path, const String& range_file_path );

    /**
    @brief Create an object which calculates quality
    @param model cv::Ptr<cv::ml::SVM> which contains a loaded GMLOG model
    @param range cv::Mat which contains GMLOG range data
    */
    CV_WRAP static Ptr<QualityBlockSVD> create( const Ptr<cv::ml::SVM>& model, const Mat& range );

    /** @brief Computes MSE for reference images supplied in class constructor and provided comparison images
    @param cmpImgs Comparison image(s)
    @returns cv::Scalar with per-channel quality values.  Values range from 0 (best) to potentially max float (worst)
    */
    CV_WRAP virtual cv::Scalar compute( InputArray cmpImgs ) CV_OVERRIDE  = 0;

    /**
    @brief method for computing quality
    @param ref reference image
    @param cmp comparison image
    @param qualityMap output quality map, or cv::noArray()
    @returns cv::Scalar with per-channel quality values.  Values range from 0 (worst) to 1 (best)
    */
    CV_WRAP virtual cv::Scalar compute( InputArray ref, InputArray cmp, OutputArray qualityMap) = 0;

    /**
    @brief method for computing quality
    @param ref reference image
    @param cmp comparison image
    @returns cv::Scalar with per-channel quality values.  Values range from 0 (worst) to 1 (best)
    */
    CV_WRAP inline cv::Scalar compute( InputArray ref, InputArray cmp )
    {
        return this->compute(ref, cmp, noArray());
    }

    /**
    @brief method that return the size of the block size used to compute the score
    */
    CV_WRAP virtual Size getBlockSize() const = 0;

    /**
    @brief method to set the block size used to compute the score
    */
    CV_WRAP virtual void setBlockSize(const Size& size) = 0;


    /**
    @brief method returning the name of the current algorithm
    */
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};

} // xquality

} //cv

#endif // NONFREE_HPP

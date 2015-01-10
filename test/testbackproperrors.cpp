#include <iostream>
#include <random>

#include "NeuralNet.h"
#include "BackpropErrors.h"

#include "gtest/gtest.h"

#include "test/gtest_supp.h"
#include "test/Sampler.h"
#include "test/WeightRandomizer.h"

using namespace std;

// This file contains tests for calculating errors for the upstream layer, whcih is currently a bit slow 
// as at the time of first creating this file :-P

// we want to test calcerrors for layer 2 in a network like:
//    NeuralNet *net = NeuralNet::maker()->planes(1)->boardSize(28)->instance();
//    net->convolutionalMaker()->numFilters(32)->filterSize(5)->relu()->biased()->insert();
//    net->convolutionalMaker()->numFilters(32)->filterSize(5)->relu()->biased()->insert();
//    net->convolutionalMaker()->numFilters(10)->filterSize(20)->tanh()->biased(config.biased)->insert();
TEST( testbackproperrors, board28 ) {
    const int batchSize = 128;
    LayerDimensions dim;
    dim.setInputPlanes( 32 ).setInputBoardSize( 28 ).setNumFilters( 32 ).setFilterSize( 5 )
        .setBiased( true ).setPadZeros( true );

    int weightsSize = dim.filtersSize;
    int biasWeightsSize = dim.numFilters;
    int resultsSize = batchSize * dim.outputCubeSize;
    float *weights = new float[max(10000, weightsSize ) ];
    float *biasWeights = new float[max( 10000, biasWeightsSize)];
    float *errors = new float[max(10000, resultsSize )];
    WeightRandomizer::randomize( weights, max(10000, weightsSize ), -1, 1 );
    WeightRandomizer::randomize( biasWeights, max( 10000, biasWeightsSize), -1, 1 );
    WeightRandomizer::randomize( errors, max(10000, resultsSize ), -1, 1 );

    OpenCLHelper cl;
    BackpropErrors *backpropErrorsImpl = BackpropErrors::instanceForTest( &cl, dim );
    Timer timer;
    float *errorsForUpstream = backpropErrorsImpl->backpropErrors( batchSize, weights, biasWeights, errors );
    timer.timeCheck("after calcing errors");

    Sampler::printSamples( "errorsForUpstream", batchSize * dim.inputCubeSize, errorsForUpstream );

    EXPECT_FLOAT_NEAR( -1.66007, errorsForUpstream[68268] );
    EXPECT_FLOAT_NEAR( 0.823709, errorsForUpstream[2927151] );
    EXPECT_FLOAT_NEAR( 6.99365, errorsForUpstream[1746549] );
    EXPECT_FLOAT_NEAR( 7.25249, errorsForUpstream[576704] );
    EXPECT_FLOAT_NEAR( 7.88787, errorsForUpstream[570179] );

    delete backpropErrorsImpl;

    delete[] errorsForUpstream;
    delete[] errors;
    delete[] weights;
    delete[] biasWeights;
}

TEST( testbackproperrors, board19 ) { // make it work for a board19 first :-)
    const int batchSize = 128;
    LayerDimensions dim;
    dim.setInputPlanes( 32 ).setInputBoardSize( 19 ).setNumFilters( 32 ).setFilterSize( 5 )
        .setBiased( true ).setPadZeros( true );

    int weightsSize = dim.filtersSize;
    int biasWeightsSize = dim.numFilters;
    int resultsSize = batchSize * dim.outputCubeSize;
    float *weights = new float[max(10000, weightsSize ) ];
    float *biasWeights = new float[max( 10000, biasWeightsSize)];
    float *errors = new float[max(10000, resultsSize )];
    WeightRandomizer::randomize( weights, max(10000, weightsSize ), -1, 1 );
    WeightRandomizer::randomize( biasWeights, max( 10000, biasWeightsSize), -1, 1 );
    WeightRandomizer::randomize( errors, max(10000, resultsSize ), -1, 1 );

    OpenCLHelper cl;
    BackpropErrors *backpropErrorsImpl = BackpropErrors::instanceForTest( &cl, dim );
    Timer timer;
    float *errorsForUpstream = backpropErrorsImpl->backpropErrors( batchSize, weights, biasWeights, errors );
    timer.timeCheck("after calcing errors");

    Sampler::printSamples( "errorsForUpstream", batchSize * dim.inputCubeSize, errorsForUpstream );

    EXPECT_FLOAT_NEAR( -24.5602, errorsForUpstream[158380] );
    EXPECT_FLOAT_NEAR( 7.39012, errorsForUpstream[2607] );
    EXPECT_FLOAT_NEAR( -6.50315, errorsForUpstream[546421] );
    EXPECT_FLOAT_NEAR( -1.22025, errorsForUpstream[429248] );
    EXPECT_FLOAT_NEAR( -8.89935, errorsForUpstream[1200963] );

    delete backpropErrorsImpl;

    delete[] errorsForUpstream;
    delete[] errors;
    delete[] weights;
    delete[] biasWeights;
}



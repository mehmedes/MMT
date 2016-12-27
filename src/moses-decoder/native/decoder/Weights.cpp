/*
 * Weights.cpp
 *
 *  Created on: 24 Oct 2015
 *      Author: hieu
 */
#include <cassert>
#include <string>
#include <vector>
#include "FF/FeatureFunction.h"
#include "FF/FeatureFunctions.h"
#include "Weights.h"
#include "System.h"
#include "legacy/Util2.h"

using namespace std;

namespace Moses2
{

Weights::Weights()
{
}

Weights::~Weights()
{
}

void Weights::Init(const FeatureFunctions &ffs)
{
  size_t totalNumScores = ffs.GetNumScores();
  //cerr << "totalNumScores=" << totalNumScores << endl;
  m_weights.resize(totalNumScores, 1);
}

std::ostream &Weights::Debug(std::ostream &out, const System &system) const
{
  const FeatureFunctions &ffs  = system.featureFunctions;
  size_t numScores = ffs.GetNumScores();
  for (size_t i = 0; i < numScores; ++i) {
    out << m_weights[i] << " ";
  }

}

std::vector<SCORE> Weights::GetWeights(const FeatureFunction &ff) const
{
  std::vector<SCORE> ret(m_weights.begin() + ff.GetStartInd(), m_weights.begin() + ff.GetStartInd() + ff.GetNumScores());
  return ret;
}

void Weights::SetWeights(const FeatureFunctions &ffs, const std::string &ffName, const std::vector<float> &weights)
{
  const FeatureFunction *ff = ffs.FindFeatureFunction(ffName);
  UTIL_THROW_IF2(ff == NULL, "Feature function not found:" << ffName);

  size_t startInd = ff->GetStartInd();
  size_t numScores = ff->GetNumScores();
  UTIL_THROW_IF2(weights.size() != numScores, "Wrong number of weights. " << weights.size() << "!=" << numScores);

  for (size_t i = 0; i < numScores; ++i) {
	SCORE weight = weights[i];
	m_weights[startInd + i] = weight;
  }
}

void Weights::SetWeights(const FeatureFunctions &ffs, const std::map<std::string, std::vector<float>> &featureWeights)
{
  for(auto w : featureWeights)
    SetWeights(ffs, w.first, w.second);
}

}

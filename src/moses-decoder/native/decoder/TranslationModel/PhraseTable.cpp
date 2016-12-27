/*
 * PhraseTable.cpp
 *
 *  Created on: 23 Oct 2015
 *      Author: hieu
 */
#include <boost/foreach.hpp>
#include <queue>
#include "PhraseTable.h"
#include "../legacy/Util2.h"
#include "../TypeDef.h"
#include "../InputType.h"
#include "../PhraseBased/Manager.h"
#include "../PhraseBased/InputPath.h"
#include "../DummySCFG/DummySCFG.h"

using namespace std;

namespace Moses2
{

////////////////////////////////////////////////////////////////////////////
PhraseTable::PhraseTable(size_t startInd, const std::string &line) :
    StatelessFeatureFunction(startInd, line), m_tableLimit(20) // default
        , m_maxCacheSize(DEFAULT_MAX_TRANS_OPT_CACHE_SIZE)
{
  m_input.push_back(0);
  // do not call ReadParameters() here -- method calls from c'tor cannot be overridden
  // instead, call it in the final child class's c'tor.
}

PhraseTable::~PhraseTable()
{
  // TODO Auto-generated destructor stub
}

void PhraseTable::SetParameter(const std::string& key, const std::string& value)
{
  if (key == "cache-size") {
    m_maxCacheSize = Scan<size_t>(value);
  }
  else if (key == "path") {
    m_path = value;
  }
  else if (key == "input-factor") {
	m_input = Tokenize<FactorType>(value, ",");
  }
  else if (key == "output-factor") {
	m_output = Tokenize<FactorType>(value, ",");
  }
  else if (key == "table-limit") {
    m_tableLimit = Scan<size_t>(value);
  }
  else {
    StatelessFeatureFunction::SetParameter(key, value);
  }
}

bool PhraseTable::SatisfyBackoff(const Manager &mgr, const InputPath &path) const
{
  const InputType &input = mgr.GetInput();
  if ((mgr.system.options.input.xml_policy == XmlExclusive)
	  && input.XmlOverlap(path.range.GetStartPos(), path.range.GetEndPos())) {
	  return false;
  }

	//cerr << GetName() << "=" << GetPtInd() << "=" << decodeGraphBackoff << endl;
	if (decodeGraphBackoff == 0) {
			// always lookup
			return true;
	}
	else if (decodeGraphBackoff == -1) {
			// lookup only if there's no existing rules
			return path.GetNumRules() ? false : true;
	}
	else if (path.range.GetNumWordsCovered() <= decodeGraphBackoff) {
			return path.GetNumRules() ? false : true;
	}

	return false;
}

void PhraseTable::Lookup(const Manager &mgr, InputPathsBase &inputPaths) const
{
  BOOST_FOREACH(InputPathBase *pathBase, inputPaths){
    InputPath *path = static_cast<InputPath*>(pathBase);
    //cerr << "path=" << path->range << " ";

    if (SatisfyBackoff(mgr, *path)) {
      TargetPhrases *tpsPtr = Lookup(mgr, mgr.GetPool(), *path);
      cerr << "tpsPtr=" << tpsPtr << " ";
      /*
       if (tps.get()) {
       cerr << tps.get()->GetSize();
       }
       cerr << endl;
       */

      path->AddTargetPhrases(*this, tpsPtr);
    }
  }

}

TargetPhrases *PhraseTable::Lookup(const Manager &mgr, MemPool &pool,
    InputPath &inputPath) const
{
  UTIL_THROW2("Not implemented");
}

void PhraseTable::EvaluateInIsolation(MemPool &pool, const System &system,
    const Phrase<Moses2::Word> &source, const TargetPhraseImpl &targetPhrase, Scores &scores,
    SCORE &estimatedScore) const
{
}

void PhraseTable::EvaluateInIsolation(MemPool &pool, const System &system, const Phrase<SCFG::Word> &source,
    const TargetPhrase<SCFG::Word> &targetPhrase, Scores &scores,
    SCORE &estimatedScore) const
{

}

// scfg
void PhraseTable::LookupUnary(MemPool &pool,
    const SCFG::Manager &mgr,
    const SCFG::Stacks &stacks,
    SCFG::InputPath &path) const
{
  UTIL_THROW2("not implemented");
}

void PhraseTable::LookupNT(
    MemPool &pool,
    const SCFG::Manager &mgr,
    const Moses2::Range &subPhraseRange,
    const SCFG::InputPath &prevPath,
    const SCFG::Stacks &stacks,
    SCFG::InputPath &outPath) const
{
  UTIL_THROW2("not implemented");
}

void PhraseTable::LookupGivenWord(
    MemPool &pool,
    const SCFG::Manager &mgr,
    const SCFG::InputPath &prevPath,
    const SCFG::Word &wordSought,
    const Moses2::Hypotheses *hypos,
    const Moses2::Range &subPhraseRange,
    SCFG::InputPath &outPath) const
{
  UTIL_THROW2("not implemented");
}

}

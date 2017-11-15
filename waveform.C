#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TH1F.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TAxis.h"

using namespace art;
using namespace std;
using namespace std::chrono;

void
waveform(std::string const& filename, std::string const& outfile,
         int nevents=1, bool onlySignal=false)
{
  InputTag daq_tag{ "daq" };
  // Create a vector of length 1, containing the given filename.
  vector<string> filenames(1, filename);

  std::ofstream fout(outfile);
  std::ofstream fout_truth(outfile+"-truth");

  int iev=0;
  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    std::set<int> channelsWithSignal;
    if(iev>=nevents) break;
    std::cout << "Event " << iev << std::endl;
    //------------------------------------------------------------------
    // Get the SimChannels so we can see where the actual energy depositions were
    auto& simchs=*ev.getValidHandle<std::vector<sim::SimChannel>>(InputTag{"largeant"});

    for(auto&& simch: simchs){
      channelsWithSignal.insert(simch.Channel());
      double charge=0;
      fout_truth << iev << " " << simch.Channel() << " ";
      for (const auto& TDCinfo: simch.TDCIDEMap()) {
        for (const sim::IDE& ide: TDCinfo.second) {
          charge += ide.numElectrons;
        } // for IDEs
        auto const tdc = TDCinfo.first;
        fout_truth << tdc << " " << charge << " ";
      } // for TDCs
      fout_truth << std::endl;
    } // loop over SimChannels

    //------------------------------------------------------------------
    // Look at the digits
    auto& digits =
      *ev.getValidHandle<vector<raw::RawDigit>>(daq_tag);
    for(auto&& digit: digits){
      if(onlySignal && channelsWithSignal.find(digit.Channel())==channelsWithSignal.end()){
        continue;
      }
      fout << iev << " " << digit.Channel() <<  " ";
      for(auto&& sample: digit.ADCs()){
        fout << sample << " ";
      }
      fout << std::endl;
    } // end loop over digits (=?channels)
    ++iev;
  } // end loop over events
}

/* 
 * File:   FairMQSampler.tpl
 * Author: winckler
 *
 * Created on February 7, 2014, 6:21 PM
 */



template <typename T1, typename T2> FairMQSampler<T1,T2>::FairMQSampler() :
  fFairRunAna(new FairRunAna()),
  fSamplerTask(new FairMQDigiLoader<T1,T2>()),
  fInputFile(""),
  fParFile(""),
  fBranch(""),
  fEventRate(1)
{
}

template <typename T1, typename T2> FairMQSampler<T1,T2>::~FairMQSampler()
{
  if(fFairRunAna) fFairRunAna->TerminateRun();
  delete fSamplerTask;
}

/// Methods

template <typename T1, typename T2> void FairMQSampler<T1,T2>::Init()
{
  FairMQDevice::Init();

  fSamplerTask->SetBranch(fBranch);
  fSamplerTask->SetTransport(fTransportFactory);

  fFairRunAna->SetInputFile(TString(fInputFile));
  TString output = fInputFile;
  output.Append(".out.root");
  fFairRunAna->SetOutputFile(output.Data());

  fFairRunAna->AddTask(fSamplerTask);

  FairRuntimeDb* rtdb = fFairRunAna->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(TString(fParFile).Data());
  rtdb->setFirstInput(parInput1);
  rtdb->print();

  fFairRunAna->Init();
  //fFairRunAna->Run(0, 0);
  FairRootManager* ioman = FairRootManager::Instance();
  fNumEvents = int((ioman->GetInChain())->GetEntries());
}

template <typename T1, typename T2> void FairMQSampler<T1,T2>::Run()
{
  FairMQLogger::GetInstance()->Log(FairMQLogger::INFO, ">>>>>>> Run <<<<<<<");
  boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

  boost::thread rateLogger(boost::bind(&FairMQDevice::LogSocketRates, this));
  boost::thread resetEventCounter(boost::bind(&FairMQSampler::ResetEventCounter, this));
  //boost::thread commandListener(boost::bind(&FairMQSampler::ListenToCommands, this));

  int sentMsgs = 0;

  boost::timer::auto_cpu_timer timer;

  cout << "Number of events to process: " << fNumEvents << endl;

  Long64_t eventNr = 0;

//  while ( fState == RUNNING ) {

  for ( /* eventNr */ ; eventNr < fNumEvents; eventNr++ ) {
    fFairRunAna->RunMQ(eventNr);

    fPayloadOutputs->at(0)->Send(fSamplerTask->GetOutput());
    sentMsgs++;

    --fEventCounter;

    while (fEventCounter == 0) {
      boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }

    if( fState != RUNNING ) { break; }
  }

  boost::this_thread::interruption_point();
//  }

  boost::timer::cpu_times const elapsed_time(timer.elapsed());

  cout << "Sent everything in:\n" << boost::timer::format(elapsed_time, 2) << endl;
  cout << "Sent " << sentMsgs << " messages!" << endl;

  //boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

  rateLogger.interrupt();
  rateLogger.join();
  resetEventCounter.interrupt();
  resetEventCounter.join();
  //commandListener.interrupt();
  //commandListener.join();
}

template <typename T1, typename T2> void FairMQSampler<T1,T2>::ResetEventCounter()
{
  while ( true ) {
    try {
      fEventCounter = fEventRate / 100;
      boost::this_thread::sleep(boost::posix_time::milliseconds(10));
    } catch (boost::thread_interrupted&) {
      cout << "resetEventCounter interrupted" << endl;
      break;
    }
  }
  FairMQLogger::GetInstance()->Log(FairMQLogger::DEBUG, ">>>>>>> stopping resetEventCounter <<<<<<<");
}

template <typename T1, typename T2> void FairMQSampler<T1,T2>::ListenToCommands()
{
  FairMQLogger::GetInstance()->Log(FairMQLogger::INFO, ">>>>>>> ListenToCommands <<<<<<<");

  bool received = false;

  while ( true ) {
    try {
      FairMQMessage* msg = fTransportFactory->CreateMessage();

      received = fPayloadInputs->at(0)->Receive(msg);

      if (received) {
        //command handling goes here.
        FairMQLogger::GetInstance()->Log(FairMQLogger::INFO, "> received command <");
        received = false;
      }

      delete msg;

      boost::this_thread::interruption_point();
    } catch (boost::thread_interrupted&) {
      cout << "commandListener interrupted" << endl;
      break;
    }
  }
  FairMQLogger::GetInstance()->Log(FairMQLogger::DEBUG, ">>>>>>> stopping commandListener <<<<<<<");
}

template <typename T1, typename T2>
void FairMQSampler<T1,T2>::SetProperty(const int key, const string& value, const int slot/*= 0*/)
{
  switch (key) {
  case InputFile:
    fInputFile = value;
    break;
  case ParFile:
    fParFile = value;
    break;
  case Branch:
    fBranch = value;
    break;
  default:
    FairMQDevice::SetProperty(key, value, slot);
    break;
  }
}

template <typename T1, typename T2>
string FairMQSampler<T1,T2>::GetProperty(const int key, const string& default_/*= ""*/, const int slot/*= 0*/)
{
  switch (key) {
  case InputFile:
    return fInputFile;
  case ParFile:
    return fParFile;
  case Branch:
    return fBranch;
  default:
    return FairMQDevice::GetProperty(key, default_, slot);
  }
}

template <typename T1, typename T2>
void FairMQSampler<T1,T2>::SetProperty(const int key, const int value, const int slot/*= 0*/)
{
  switch (key) {
  case EventRate:
    fEventRate = value;
    break;
  default:
    FairMQDevice::SetProperty(key, value, slot);
    break;
  }
}

template <typename T1, typename T2>
int FairMQSampler<T1,T2>::GetProperty(const int key, const int default_/*= 0*/, const int slot/*= 0*/)
{
  switch (key) {
  case EventRate:
    return fEventRate;
  default:
    return FairMQDevice::GetProperty(key, default_, slot);
  }
}



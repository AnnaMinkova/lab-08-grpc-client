#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "suggest.grpc.pb.h"
#include "suggest.pb.h"

using grpc::ClientContext;
using grpc::Status;
using suggest::SuggestRequest;
using suggest::Suggest;
using suggest::SuggestResponse;
using suggest::Suggest;

int main(int argc, char** argv) {
  if (argc == 1) {
    throw std::invalid_argument("Empty query");
  }
  auto channel = grpc::CreateChannel("0.0.0.0:9090", grpc::InsecureChannelCredentials());
  auto client = suggest::Suggest::NewStub(channel);
  SuggestResponse response;
  ClientContext context;
  SuggestRequest request;
  request.set_input(argv[1]);
  Status status = client->SuggestHandler(&context, request, &response);

  if (status.ok()) {
    nlohmann::json jsonOutput;
    jsonOutput["suggestions"] = {};
    for (const auto& suggest : response.suggest_answer()) {
      nlohmann::json element;
      element["text"] = suggest.text();
      element["position"] = suggest.position();
      jsonOutput["suggestions"].push_back(element);
    }
    std::cout << std::setw(2) << jsonOutput << std::endl;
  } else {
    return -1;
  }
}

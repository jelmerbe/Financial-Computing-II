file(GLOB PROJECT_SOURCE_FILES "Src/*.cpp")
file(GLOB INCLUDE_FILES "*.hpp" "Inline/*.hpp")
file(GLOB DOC_FILES "*.hpp")
add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCE_FILES} ${INCLUDE_FILES}
        ../Session1/Src/Session1.cpp
        ../Homework1/Src/Homework1.cpp
        ../Session2/Src/Session2.cpp
        ../Homework2/Src/Homework2.cpp
        ../Session3/Src/Session3.cpp
        ../Homework3/Src/volatilityBlack.cpp
        ../Homework3/Src/discountSvensson.cpp
        ../Homework3/Src/discountRate.cpp
        ../Homework3/Src/forwardFXCarry.cpp
        ../Session4/Src/downOutCall.cpp
        ../Session4/Src/callOnForwardPrice.cpp
        ../Session4/Src/americanButterfly.cpp
        ../Session4/Src/corridor.cpp
        ../Homework4/Src/straddle.cpp
        ../Homework4/Src/americanCallOnForward.cpp
        ../Homework4/Src/downRebate.cpp
        ../Homework4/Src/upInAmericanPut.cpp
        ../Homework5/Src/collar.cpp
        ../Homework5/Src/americanSwaption.cpp
        ../Homework5/Src/putCallBond.cpp
        ../Homework5/Src/americanPutOnFutures.cpp
        ../Session5/Src/Session5.cpp
        ../Review1/Src/yieldVasicek.cpp
        ../Review1/Src/forwardAnnuity.cpp
        ../Review1/Src/yieldTMAnnuity.cpp
        ../Review1/Src/forwardYTMAnnuity.cpp
        ../Review1/Src/forwardLogLinInterp.cpp
        ../Review1/Src/forwardFXCarryLinInterp.cpp
        ../Review1/Src/forwardBlack2Fit.cpp
        ../Review1/Src/discountSwapFit.cpp
        ../ReviewExam/Src/durationAnnuity.cpp
        ../ReviewExam/Src/durationRateLogLinInterp.cpp
        ../ReviewExam/Src/swingWithPenaltyPaidAtMaturity.cpp
        ../ReviewExam/Src/swapArrears.cpp
        ../Review2/Src/boost.cpp
        ../Review2/Src/clique.cpp
        ../Review2/Src/simpleSwingWithPenalty.cpp
        ../Review2/Src/americanChooser.cpp
        ../Review2/Src/upRangeOutPut.cpp
        ../Review2/Src/varianceSwapStrike.cpp
        ../Review2/Src/assetSwaption.cpp
        ../Review2/Src/fxCancellableSwap.cpp)


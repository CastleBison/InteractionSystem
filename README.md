# Interaction System

언리얼 엔진에서 **플레이어와 오브젝트 간의 상호작용(Interaction)** 을 구현하고 학습한 내용을 정리한 레포지토리입니다.  
C++ 기반의 상호작용 시스템 제작 과정을 단계별로 기록했습니다.

---

##  프로젝트 개요

- **프로젝트명:** Interaction System  
- **엔진 버전:** Unreal Engine 5.6 
- **언어:** C++ / Blueprint  
- **학습 목적:**  
  - C++로 인터랙션 로직 구조 이해

---
##  자동문 (DoorBase)

### 구현 개요
플레이어가 문 앞에 다가가면 자동으로 열리고, 일정 시간 후 닫히는 시스템을 C++로 구현했습니다.  
언리얼 블루프린트의 `OnComponentBeginOverlap` / `OnComponentEndOverlap` 이벤트를 C++로 옮겨  
보다 단순하고 효율적으로 동작하도록 구성했습니다.

### 주요 기능
- `OverlapBox`로 플레이어 감지  
- `CheckDir()` 함수로 문 정면/후면 방향 판별  
- `FInterpTo`를 사용해 문이 부드럽게 회전하도록 구현  
- 플레이어가 영역을 벗어나면 **3초 후 자동으로 닫힘**  
- 타임라인, 커브 없이 Tick 기반으로 간소화된 동작

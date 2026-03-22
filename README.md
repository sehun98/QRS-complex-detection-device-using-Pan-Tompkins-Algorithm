

심전도 측정 기기를 설계하기에 앞서, 인체의 신경 세포와 근육 세포가 전기적 신호를 생성하는 과정인 활동 전위(Action Potential)를 이해하는 것이 중요하다. 

이러한 활동 전위를 통해 발생하는 심전도 파형 중 하나인 QRS complex는 심장의 전기적 활동을 반영하며, 심부전, 심근경색, 심장비대와 같은 다양한 심장 질환을 진단하는 데 중요한 역할을 한다.

또한, 산소포화도(SPO2)와 함께 측정하면 심장 건강과 산소 공급 능력을 종합적으로 평가할 수 있어, 여러 질병을 조기에 발견하는 데 큰 도움이 된다.

## Active Potential

------

![action_potential.png](./images/action_potential.png)

1. Rest membrane potential : Extracellular fluid에 Na⁺ > Cl ⁻ > K ⁺  순으로 함유되어있다. 이때 extra에 있는 3개의 Sodium(Na⁺ )와 intra에 있는 2개의 Potassium(K ⁺ )이 Na⁺/K⁺ ATPase에 의해 교환이 이루어진다. 이를 -80mV의 음전하를 띄는 Rest membrane potential이라 한다.
2. Depolarization : 여러 자극이 존재하지만 근육에 붙어있는 Tendon에서 신호가 발생한 근 신호를 예를 들어 자극이 임계값(threshold)이상 발생하면 Na⁺  channel이 열리게 되어 Na⁺ 가 농도 차이에 의해 빠르게 유입되어 전위가 약 +30mV까지 상승하게 된다. 이를 Depolarization 이라 한다.
3. Repolarization : Depolarization 직후 Na⁺  channel이 닫히고 K⁺  channel이 열리게 되어 Potassium(K ⁺ )이 세포 밖으로 빠져나가 음전하로 떨어지게 된다.
4. After potential : Na⁺  channel이 원상태로 돌아갈때까지 K⁺  channel이 닫히지 않아 Potassium(K ⁺ )이 과도하게 세포 밖으로 빠져나가 과분극(Hyperpolarization) 이 발생한다. 이를 after potential이라고도 한다.

이러한 과정에 신호 전달 과정이 신경 세포에서 이루어진다.

## QRS파

------

### 심장의 전기신호 전달과정

<div align="center">  <img src="./images/action_potential2.png" alt="action_potential2" width="200" /></div>

1. SA Node에서 시작된 전기 신호 : 심방을 따라 신호가 전달되어 심방을 수축시킨다. 이때 심방이 탈분극이 되지만 QRS파에는 나타나지 않고 QRS파는 심실의 탈분극을 나타내기 때문에, 심방 수축 시에는 P파가 나타난다.
2. AV Node에서 신호 지연 : 심방이 수축하고 심실로 혈액을 충분히 보낼 수 있도록 확보한다.
3. AV node를 통해 His bundle로 심실 수축 관여 : His bundle을 통해 전달된 신호는 심실 사이의 벽 수축을 일으켜 Q파가 발생한다.
4. Purkinje fibers를 통해 심실 전체 수축 : 가장 빠르고 강하게 수축하게 하는 Purkinje fibers를 통해 심실 전체가 수축하여 R파가 크게 발생하고 심실벽의 마지막 수축으로 S파가 발생한다.
5. 심실의 이완으로 T파가 발생한다.

P파 (P-wave) : 심방의 수축

**Q파 (Q-wave)** : 심실벽의 최초 수축 **R파 (R-wave)** : 심실 전체 수축 **S파 (S-wave)** : 심실벽의 마지막 수축

T파 (S-wave) :  심실의 이완

## Pan Tompkins Algorithm

------

BPM을 계산하기 위해 심전도(ECG) 신호에서 QRS 복합체를 검출하는 알고리즘

<div align="center">  <img src="./images/pan_tompkins_algorithm.png" alt="pan_tompkins_algorithm" /></div>

### 실제 측정 데이터와 Pan Tompkins Algorithm 결과

<div align="center">  <img src="./images/pan_tompkins_algorithm_result.png" alt="pan_tompkins_algorithm_result" /></div>



<div align="center">  <img src="./images/figure1.png" alt="figure1" width="300"/></div>

<div align="center">  <img src="./images/figure2.png" alt="figure2" width="300"/></div>

[ECG_Serial.m](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/ba46b4e6-6684-4396-b4ba-d1cb884583c2/ECG_Serial.m)

```c
% UART 데이터 수신 및 실시간 플롯 (데이터 수신 속도 개선)
clear; clc;

%% 1. UART 포트 설정
portName = 'COM7';  % UART 포트 이름
baudRate = 115200;  % UART 통신 속도

% UART 포트 열기
serialObj = serialport(portName, baudRate);
configureTerminator(serialObj, 'LF'); % '\\n' 문자 기준 데이터 수신
serialObj.InputBufferSize = 8192; % 버퍼 크기 증가

disp('UART 포트가 열렸습니다. 데이터를 수신 중입니다...');

%% 2. 실시간 플롯 준비
figure;
grid on;
hold on;
title('UART 데이터 실시간 플롯 (첫 번째 숫자)');
xlabel('Time (s)');
ylabel('Value');
ylim([450 550]);  % Y축 데이터 범위를 400~500으로 설정
xlim([0 10]);     % 시간 범위 (필요시 수정)

% 데이터 저장용 변수
data = [];
timestamps = [];
startTime = tic;  % 시간 측정 시작

%% 3. 데이터 수신 및 플롯
while toc(startTime) < 10 % 10초 동안 데이터를 수신
    if serialObj.NumBytesAvailable > 0
        % UART 데이터 읽기
        rawData = readline(serialObj); % UART 데이터 한 줄 읽기
        disp(['Received Data: ', rawData]); % 수신된 데이터 확인
        
        % 1. 특수 문자 및 비숫자 제거
        cleanData = regexprep(rawData, '[^\\d\\s]', ''); % 숫자와 공백만 유지
        disp(['Cleaned Data: ', cleanData]); % 정리된 데이터 표시
        
        % 2. 공백 기준으로 데이터 분리
        numbers = split(strtrim(cleanData)); % 공백 제거 후 분리
        
        % 3. 첫 번째 숫자 추출
        if ~isempty(numbers) && ~isnan(str2double(numbers{1}))
            value = str2double(numbers{1}); % 첫 번째 숫자 추출
            disp(['Parsed Value: ', num2str(value)]); % 추출된 값 확인
            
            % 현재 시간 및 데이터 저장
            elapsedTime = toc(startTime);
            data = [data, value];
            timestamps = [timestamps, elapsedTime];
            
            % 플롯 업데이트 (drawnow를 최소화)
            if mod(length(data), 10) == 0 % 10번째 데이터마다 업데이트
                plot(timestamps, data, '-o', 'LineWidth', 1.5);
                drawnow limitrate; % 화면 업데이트 최적화
            end
        else
            disp('유효하지 않은 데이터입니다.'); % 유효하지 않은 데이터 디버깅
        end
    end
end

disp('데이터 수신이 완료되었습니다.');

%% 4. UART 포트 닫기
delete(serialObj);
clear serialObj;
disp('UART 포트가 닫혔습니다.');
```

# Signal Process in LabVIEW

------

<div align="center">  <img src="./images/LabVIEW.png" alt="LabVIEW" /></div>

<div align="center">  <img src="./images/LabVIEW2.png" alt="LabVIEW2" /></div>

# Objective

------

- 소프트웨어와 신호처리를 적용해보기 위한 QRS complex 감지 의료장비 설계

# Description

------

- 그동안의 프로젝트에서 학습한 기술을 활용한 심전도 프로젝트
- 노이즈 감소를 목표로 Right Leg Driver를 도입한 설계를 수행하였으며, Half-Cell Potential에 대한 경험
- Pan Tompkins Algorithm을 성공적으로 적용하여 데이터 처리와 분석에 활용

# Features

------

- The Atmel AVR 8-bit Microcontrollers Atmega128
- Instrument OP-AMP
- Right Leg Driver
- DC Offset Summing Amplifier
- 2 Gain Amplifier
- 5V to 3V3 Level Shifter
- TFT-LCD

# Circuits

------

![스크린샷 2024-11-10 215514.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/8173acca-56f8-4fad-9ef2-a8f9f5b314c2/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215514.png)

## 1. MCU / +5V Power / -5V Power / +3V3 Power

![스크린샷 2024-11-10 215524.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/66a41c89-6493-4559-a330-e13222ea6015/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215524.png)

### 1.1 MCU (ATmega128) :

- Description : 8-bit AVR microcontroller
- Operating Voltage : 3.3V
- Communication : SPI / 10 bit resolution ADC

### 1.2 +5V Power / +3V3 Power :

- Description : 리니어 레귤레이터 LM7805 / LM3940

### 1.3 -5V Power :

- Description : 스위칭 레귤레이터  ICL7660

## 2. Electrocardiogram

![스크린샷 2024-11-10 215533.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/c1a6871c-bf9b-4630-9c81-4897c288c2e0/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215533.png)

### 2.1 Electrocardiogram :

- Description : Non-invert Amp + Differential Amp + Driven Right Leg + Active 1-pole band pass filter + Summing Amp
- Communication : 10 bit resolution ADC

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/0d4bd5fe-8a0c-4fb4-b1eb-b0fe4f16ae59/image.png)

- Non-invert Amp : Differential Amp만 사용하게 될 경우 Lead 저항과 합성이 되어 impedance isolation을 진행해야 한다.

  $$ \: A_V = 1 + \frac{R_1}{R_{\text{gain}/2}} = 1 + \frac{2R_1}{R_{\text{gain}}} $$

  $$ \: CMRR \approx 1 $$

- Differential Amp : CMRR이 좋은 소신호 증폭기

  $$ V_{\text{out}} = V_1 \times \left( 1 + \frac{2R_1}{R_{\text{gain}}} \right) \left( - \frac{R_3}{R_2} \right) $$

  $$ \: CMRR \approx \infty

  $$

- Driven Right Leg : 작은 전류로 CMRR을 feedback 시켜 CMRR을 효과적으로 제거하기 위해 사용되고 R4를 통해 인체에 흘러가는 전류량을 제한한다.

  $$ \: A_V = \frac{1}{2} \left( - \frac{R_3}{R_1} \right) = -100

  $$

  $$ \: V_{\text{out}} = 2 \times \frac{1}{2} \left( V_{\text{in}} + 1.24V_{\text{DC}} \right) $$

- Active 1-pole band pass filter : band pass filter를 통해 신호를 필터한다.

  $$ \: f_{CH} = \frac{1}{2\pi C_1 R_1} \approx 0.048\text{Hz}, \quad f_{CL} = \frac{1}{2\pi R_4 C_2} \approx 106.1\text{Hz}

  $$

  $$ \: A_V = 1 + \frac{R_4}{R_3} = 4.19

  $$

  $$ \: R_2 = R_1 = 3.3\text{Mohm} \quad \text{(impedance matching)}

  $$

- Summing Amp : MCU에서 인식할 수 있는 전압 범위를 위해 offset을 진행하며 증폭을 진행 시킨다.

  $$ V_{\text{out}} = 2 \times \frac{1}{2} \left( V_{\text{in}} + 1.24 V_{\text{DC}} \right) $$

## 3. 5V to 3V3 Level Shifter / TFT LCD / LEAD Connector

![스크린샷 2024-11-10 215545.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/80a8d157-27f7-457c-864f-a5867d489a0b/%EC%8A%A4%ED%81%AC%EB%A6%B0%EC%83%B7_2024-11-10_215545.png)

### 3.1 Level Shifter 5V to 3V3 :

- Description : 8-bit AVR microcontroller는 GPIO 전압이 5V이므로 LCD를 구동하기 위해서는 3V3 변환이 필요
- Resistor (R23) 10kΩ : Pull-up 시 A to B Shift 진행

### 3.2 TFT LCD :

- Description : SZH-EK096 TFT LCD
- Operating Voltage : 3.3V
- Communication : SPI

### 3.3 LEAD Connector :

- Description : 심전도 3 Lead Connector RA : 오른팔 LA : 왼팔 LL : 오른다리

### 주요 심전도 Lead

- **1-리드**: 웨어러블 기기, 간단한 심장 리듬 감지
- **3-리드**: 응급 상황에서 빠른 리듬 체크
- **5-리드**: 병원 모니터링, 실시간 심장 리듬 확인
- **6-리드**: 간이 심전도, 기본적인 심장 상태 확인
- **12-리드**: 표준 진단용, 심장의 다양한 부위를 평가 가능

# Part Layout

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/4c0f8206-5be8-4f73-a8f1-8cf785c6080f/image.png)

# Top Layer

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/b6bede03-f119-418a-9292-962c65e78ca3/image.png)

# Bottom Layer

------

![image.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/d7fd048e-45c5-49ab-b234-d81b16a92994/808b1eea-3e37-4c74-93df-05e04986074e/image.png)

# 프로젝트 진행하면서 어려웠던 점

------

[문제점]

Common Mode Rejection Ratio와 Right Leg Driver를 활용한 노이즈 저감 이론

[해결방안]

의용계측 수업 및 의용계측공학 서적을 통해 CMRR 이론 학습

[문제점]

1코어 동작 시 UART 전송 동안 Heart Rate Sampling이 늦어지는 문제

[해결방안]

UART Baud Rate를 높히고, ECG 측정 후 임시 버퍼에 저장하여 데이터 유실을 줄임

# 프로젝트 진행하면서 깨달은 점

------

[깨달은점]

Half-Cell Potential에 의한 R-R interval 시 Threshold를 동적으로 반응 해야함
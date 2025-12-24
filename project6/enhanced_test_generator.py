import random
import numpy as np
from pathlib import Path
import json
from datetime import datetime

class TexturePackingTestGenerator:
    """Generate comprehensive test cases for texture packing algorithms"""
    
    def __init__(self, output_dir="test_cases_enhanced"):
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)
        
        # Define test configurations
        self.test_configs = {
            'small_scale': {
                'sizes': [10, 20, 50, 100],
                'widths': [500, 1000, 2000],
                'distributions': ['uniform', 'normal', 'exponential', 'mixed']
            },
            'medium_scale': {
                'sizes': [200, 500, 1000, 2000],
                'widths': [1000, 2000, 5000],
                'distributions': ['uniform', 'normal', 'exponential', 'mixed', 'clustered']
            },
            'large_scale': {
                'sizes': [3000, 5000, 8000, 10000],
                'widths': [2000, 5000, 10000],
                'distributions': ['uniform', 'normal', 'mixed', 'clustered']
            }
        }
        
        # Distribution parameters
        self.distribution_params = {
            'uniform': {
                'width_range': (10, 200),
                'height_range': (10, 150),
                'description': 'Uniform distribution of rectangle dimensions'
            },
            'normal': {
                'width_mean': 80, 'width_std': 30,
                'height_mean': 60, 'height_std': 20,
                'description': 'Normal distribution with moderate variance'
            },
            'exponential': {
                'width_lambda': 0.02,
                'height_lambda': 0.025,
                'max_width': 200, 'max_height': 150,
                'description': 'Exponential distribution favoring smaller rectangles'
            },
            'mixed': {
                'components': [
                    {'type': 'uniform', 'weight': 0.4, 'params': {'width_range': (20, 100), 'height_range': (20, 80)}},
                    {'type': 'normal', 'weight': 0.4, 'params': {'width_mean': 120, 'width_std': 40, 'height_mean': 90, 'height_std': 30}},
                    {'type': 'exponential', 'weight': 0.2, 'params': {'width_lambda': 0.01, 'height_lambda': 0.015, 'max_width': 150, 'max_height': 120}}
                ],
                'description': 'Mixed distribution combining multiple patterns'
            },
            'clustered': {
                'clusters': 3,
                'cluster_params': [
                    {'center': (40, 30), 'spread': 15, 'weight': 0.3},
                    {'center': (100, 75), 'spread': 25, 'weight': 0.4},
                    {'center': (160, 120), 'spread': 20, 'weight': 0.3}
                ],
                'description': 'Clustered distribution with distinct size groups'
            }
        }
        
        self.generated_files = []
        self.test_metadata = {}
        
    def generate_uniform_rectangles(self, n, width_range, height_range):
        """Generate rectangles with uniform distribution"""
        rectangles = []
        for i in range(n):
            width = random.randint(width_range[0], width_range[1])
            height = random.randint(height_range[0], height_range[1])
            rectangles.append((width, height))
        return rectangles
    
    def generate_normal_rectangles(self, n, width_mean, width_std, height_mean, height_std):
        """Generate rectangles with normal distribution"""
        rectangles = []
        for i in range(n):
            width = int(np.random.normal(width_mean, width_std))
            height = int(np.random.normal(height_mean, height_std))
            # Ensure positive dimensions
            width = max(10, min(300, width))
            height = max(10, min(200, height))
            rectangles.append((width, height))
        return rectangles
    
    def generate_exponential_rectangles(self, n, width_lambda, height_lambda, max_width, max_height):
        """Generate rectangles with exponential distribution"""
        rectangles = []
        for i in range(n):
            width = int(np.random.exponential(1/width_lambda))
            height = int(np.random.exponential(1/height_lambda))
            # Cap dimensions
            width = min(width, max_width)
            height = min(height, max_height)
            width = max(10, width)
            height = max(10, height)
            rectangles.append((width, height))
        return rectangles
    
    def generate_mixed_rectangles(self, n, components):
        """Generate rectangles with mixed distribution"""
        rectangles = []
        for i in range(n):
            # Choose component based on weights
            rand_val = random.random()
            cumulative_weight = 0
            selected_component = None
            
            for component in components:
                cumulative_weight += component['weight']
                if rand_val <= cumulative_weight:
                    selected_component = component
                    break
            
            if selected_component['type'] == 'uniform':
                width_range = selected_component['params']['width_range']
                height_range = selected_component['params']['height_range']
                width = random.randint(width_range[0], width_range[1])
                height = random.randint(height_range[0], height_range[1])
            elif selected_component['type'] == 'normal':
                width_mean = selected_component['params']['width_mean']
                width_std = selected_component['params']['width_std']
                height_mean = selected_component['params']['height_mean']
                height_std = selected_component['params']['height_std']
                width = int(np.random.normal(width_mean, width_std))
                height = int(np.random.normal(height_mean, height_std))
                width = max(10, min(300, width))
                height = max(10, min(200, height))
            else:  # exponential
                width_lambda = selected_component['params']['width_lambda']
                height_lambda = selected_component['params']['height_lambda']
                max_width = selected_component['params']['max_width']
                max_height = selected_component['params']['max_height']
                width = int(np.random.exponential(1/width_lambda))
                height = int(np.random.exponential(1/height_lambda))
                width = min(width, max_width)
                height = min(height, max_height)
                width = max(10, width)
                height = max(10, height)
            
            rectangles.append((width, height))
        return rectangles
    
    def generate_clustered_rectangles(self, n, clusters, cluster_params):
        """Generate rectangles with clustered distribution"""
        rectangles = []
        for i in range(n):
            # Choose cluster based on weights
            rand_val = random.random()
            cumulative_weight = 0
            selected_cluster = None
            
            for cluster in cluster_params:
                cumulative_weight += cluster['weight']
                if rand_val <= cumulative_weight:
                    selected_cluster = cluster
                    break
            
            center_w, center_h = selected_cluster['center']
            spread = selected_cluster['spread']
            
            # Generate rectangle around cluster center
            width = int(np.random.normal(center_w, spread))
            height = int(np.random.normal(center_h, spread))
            
            # Ensure reasonable bounds
            width = max(10, min(250, width))
            height = max(10, min(180, height))
            
            rectangles.append((width, height))
        return rectangles
    
    def generate_test_case(self, n, bin_width, distribution_type):
        """Generate a complete test case"""
        if distribution_type == 'uniform':
            params = self.distribution_params['uniform']
            rectangles = self.generate_uniform_rectangles(
                n, params['width_range'], params['height_range'])
        elif distribution_type == 'normal':
            params = self.distribution_params['normal']
            rectangles = self.generate_normal_rectangles(
                n, params['width_mean'], params['width_std'],
                params['height_mean'], params['height_std'])
        elif distribution_type == 'exponential':
            params = self.distribution_params['exponential']
            rectangles = self.generate_exponential_rectangles(
                n, params['width_lambda'], params['height_lambda'],
                params['max_width'], params['max_height'])
        elif distribution_type == 'mixed':
            params = self.distribution_params['mixed']
            rectangles = self.generate_mixed_rectangles(n, params['components'])
        elif distribution_type == 'clustered':
            params = self.distribution_params['clustered']
            rectangles = self.generate_clustered_rectangles(
                n, params['clusters'], params['cluster_params'])
        else:
            raise ValueError(f"Unknown distribution type: {distribution_type}")
        
        return {
            'bin_width': bin_width,
            'num_rectangles': n,
            'distribution': distribution_type,
            'rectangles': rectangles,
            'statistics': {
                'avg_width': sum(w for w, h in rectangles) / len(rectangles),
                'avg_height': sum(h for w, h in rectangles) / len(rectangles),
                'min_width': min(w for w, h in rectangles),
                'max_width': max(w for w, h in rectangles),
                'min_height': min(h for w, h in rectangles),
                'max_height': max(h for w, h in rectangles),
                'total_area': sum(w * h for w, h in rectangles)
            }
        }
    
    def save_test_case(self, test_case, filename):
        """Save test case to file"""
        filepath = self.output_dir / filename
        
        with open(filepath, 'w') as f:
            # Write header: bin_width and number of rectangles
            f.write(f"{test_case['bin_width']} {test_case['num_rectangles']}\n")
            
            # Write rectangles
            for width, height in test_case['rectangles']:
                f.write(f"{width} {height}\n")
        
        return filepath
    
    def generate_all_test_cases(self):
        """Generate all test cases across all scales and distributions"""
        print("=== Generating Comprehensive Test Cases ===")
        
        test_metadata = {
            'generation_info': {
                'timestamp': datetime.now().isoformat(),
                'generator_version': '1.0',
                'total_test_cases': 0
            },
            'test_cases': {},
            'distribution_summary': {}
        }
        
        total_cases = 0
        
        for scale_name, config in self.test_configs.items():
            print(f"\nGenerating {scale_name} test cases...")
            
            for distribution in config['distributions']:
                print(f"  Distribution: {distribution}")
                
                for width in config['widths']:
                    for size in config['sizes']:
                        # Generate test case
                        test_case = self.generate_test_case(size, width, distribution)
                        
                        # Create filename
                        filename = f"test_enhanced_{distribution}_w{width}_n{size}.txt"
                        
                        # Save to file
                        filepath = self.save_test_case(test_case, filename)
                        self.generated_files.append(filepath)
                        
                        # Store metadata
                        test_metadata['test_cases'][filename] = {
                            'scale': scale_name,
                            'distribution': distribution,
                            'bin_width': width,
                            'num_rectangles': size,
                            'statistics': test_case['statistics'],
                            'distribution_description': self.distribution_params[distribution]['description']
                        }
                        
                        total_cases += 1
                        print(f"    Generated: {filename}")
        
        # Update total count
        test_metadata['generation_info']['total_test_cases'] = total_cases
        
        # Generate distribution summary
        for distribution in self.distribution_params.keys():
            distribution_cases = [
                case for case in test_metadata['test_cases'].values() 
                if case['distribution'] == distribution
            ]
            
            if distribution_cases:
                avg_width = sum(case['statistics']['avg_width'] for case in distribution_cases) / len(distribution_cases)
                avg_height = sum(case['statistics']['avg_height'] for case in distribution_cases) / len(distribution_cases)
                
                test_metadata['distribution_summary'][distribution] = {
                    'total_cases': len(distribution_cases),
                    'avg_rectangle_width': avg_width,
                    'avg_rectangle_height': avg_height,
                    'description': self.distribution_params[distribution]['description']
                }
        
        # Save metadata
        metadata_path = self.output_dir / "test_metadata.json"
        with open(metadata_path, 'w') as f:
            json.dump(test_metadata, f, indent=2)
        
        self.test_metadata = test_metadata
        
        print(f"\n=== Test Case Generation Complete ===")
        print(f"Total test cases generated: {total_cases}")
        print(f"Files saved to: {self.output_dir}")
        print(f"Metadata saved to: {metadata_path}")
        
        return test_metadata
    
    def generate_edge_cases(self):
        """Generate special edge case test scenarios"""
        print("\n=== Generating Edge Cases ===")
        
        edge_cases = [
            {
                'name': 'uniform_small_squares',
                'description': 'Many small uniform squares',
                'params': {'n': 1000, 'width': 1000, 'distribution': 'uniform', 
                          'width_range': (10, 20), 'height_range': (10, 20)}
            },
            {
                'name': 'extreme_aspect_ratios',
                'description': 'Rectangles with extreme aspect ratios',
                'params': {'n': 500, 'width': 2000, 'distribution': 'mixed',
                          'components': [
                              {'type': 'uniform', 'weight': 0.5, 'params': {'width_range': (150, 200), 'height_range': (10, 30)}},
                              {'type': 'uniform', 'weight': 0.5, 'params': {'width_range': (10, 30), 'height_range': (150, 200)}}
                          ]}
            },
            {
                'name': 'large_rectangles',
                'description': 'Few large rectangles',
                'params': {'n': 50, 'width': 1000, 'distribution': 'uniform',
                          'width_range': (100, 300), 'height_range': (100, 200)}
            },
            {
                'name': 'degenerate_case',
                'description': 'Single rectangle filling most of the bin',
                'params': {'n': 1, 'width': 1000, 'distribution': 'uniform',
                          'width_range': (900, 950), 'height_range': (900, 950)}
            }
        ]
        
        for edge_case in edge_cases:
            print(f"  Generating edge case: {edge_case['name']}")
            
            # Generate test case with special parameters
            params = edge_case['params']
            if 'distribution' in params and params['distribution'] == 'uniform':
                rectangles = self.generate_uniform_rectangles(
                    params['n'], params['width_range'], params['height_range'])
            elif 'components' in params:
                rectangles = self.generate_mixed_rectangles(params['n'], params['components'])
            else:
                # Default to uniform
                rectangles = self.generate_uniform_rectangles(params['n'], (20, 100), (20, 80))
            
            test_case = {
                'bin_width': params['width'],
                'num_rectangles': params['n'],
                'distribution': 'edge_case',
                'rectangles': rectangles,
                'statistics': {
                    'avg_width': sum(w for w, h in rectangles) / len(rectangles),
                    'avg_height': sum(h for w, h in rectangles) / len(rectangles),
                    'min_width': min(w for w, h in rectangles),
                    'max_width': max(w for w, h in rectangles),
                    'min_height': min(h for w, h in rectangles),
                    'max_height': max(h for w, h in rectangles),
                    'total_area': sum(w * h for w, h in rectangles)
                }
            }
            
            filename = f"edge_case_{edge_case['name']}.txt"
            filepath = self.save_test_case(test_case, filename)
            self.generated_files.append(filepath)
            
            # Add to metadata
            if 'test_cases' not in self.test_metadata:
                self.test_metadata['test_cases'] = {}
            
            self.test_metadata['test_cases'][filename] = {
                'scale': 'edge_case',
                'distribution': 'edge_case',
                'edge_case_type': edge_case['name'],
                'description': edge_case['description'],
                'bin_width': params['width'],
                'num_rectangles': params['n'],
                'statistics': test_case['statistics']
            }
            
            print(f"    Generated: {filename}")
    
    def create_test_case_summary(self):
        """Create a summary report of all generated test cases"""
        if not self.test_metadata:
            print("No test metadata available. Run generate_all_test_cases first.")
            return
        
        print("\n=== Creating Test Case Summary ===")
        
        summary_path = self.output_dir / "test_case_summary.txt"
        
        with open(summary_path, 'w') as f:
            f.write("=== TEXTURE PACKING TEST CASE GENERATION SUMMARY ===\n\n")
            f.write(f"Generation Date: {self.test_metadata['generation_info']['timestamp']}\n")
            f.write(f"Total Test Cases: {self.test_metadata['generation_info']['total_test_cases']}\n\n")
            
            f.write("=== DISTRIBUTION ANALYSIS ===\n")
            for dist_name, dist_info in self.test_metadata['distribution_summary'].items():
                f.write(f"\n{dist_name.upper()} Distribution:\n")
                f.write(f"  Description: {dist_info['description']}\n")
                f.write(f"  Total Cases: {dist_info['total_cases']}\n")
                f.write(f"  Average Rectangle Size: {dist_info['avg_rectangle_width']:.1f} x {dist_info['avg_rectangle_height']:.1f}\n")
            
            f.write("\n=== TEST CASE BREAKDOWN ===\n")
            
            # Group by scale
            scales = {}
            for filename, case_info in self.test_metadata['test_cases'].items():
                scale = case_info.get('scale', 'unknown')
                if scale not in scales:
                    scales[scale] = []
                scales[scale].append((filename, case_info))
            
            for scale, cases in scales.items():
                f.write(f"\n{scale.upper()} Scale ({len(cases)} cases):\n")
                for filename, case_info in cases[:5]:  # Show first 5 cases
                    f.write(f"  {filename}: {case_info['num_rectangles']} rectangles, "
                           f"bin width {case_info['bin_width']}, "
                           f"distribution: {case_info['distribution']}\n")
                if len(cases) > 5:
                    f.write(f"  ... and {len(cases) - 5} more cases\n")
        
        print(f"Test case summary saved to: {summary_path}")
        return summary_path

if __name__ == "__main__":
    # Generate comprehensive test cases
    generator = TexturePackingTestGenerator()
    
    # Generate main test cases
    metadata = generator.generate_all_test_cases()
    
    # Generate edge cases
    generator.generate_edge_cases()
    
    # Create summary
    generator.create_test_case_summary()
    
    print("\n=== TEST CASE GENERATION COMPLETED ===")
    print("Enhanced test cases are ready for algorithm evaluation.")
    print("Next step: Run the algorithms on these test cases and analyze results.")